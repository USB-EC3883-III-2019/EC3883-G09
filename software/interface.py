#!python 3.x
#################################################
## Author       : Jose Moran
## Github       : jose0796
## email        : jmoran071996@gmail.com
## file         : interface.py 
## Description  : Implementation of a Graphical 
## User Interface for serial communication and 
## plotting of data received from MC9S08QE128 
## microprocessor for SONAR/LIDAR.  
#################################################


import sys 
import numpy as np
import math
from PyQt5 import QtGui, QtWidgets, QtCore
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.animation as animation


class MplCanvas(FigureCanvas):
    """ 
    
        Matplotlib Canvas object for creating plotting window

    """

    def __init__(self, parent=None, width=1, height=1, dpi=100):

        #Plot config
        self.fig = plt.figure()  
        FigureCanvas.__init__(self, self.fig)
        self.axes = self.fig.add_subplot(111)
        #hide x's and y's labels 
        self.axes.set_yticklabels([])
        self.axes.set_xticklabels([])

        #Grid configuration
        
        self.axes.grid(b=True,which='major',color='k',linestyle='-')
        self.axes.minorticks_on()
        self.axes.grid(b=True,which='minor',linestyle='--')
        
        
        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(parent) #really important to do this for the gui to show image
        self.canvas.move(60,80) 

        
        
        
        self.xticks = np.linspace(0,1,10) #these both are based on project requirements
        self.yticks = np.linspace(0,3,10)
        self.axes.xaxis.set_ticks(self.xticks) 
        self.axes.yaxis.set_ticks(self.yticks)

        #open serial port for serial communication with DEM0QE
        # self.dataSerial = openPort(listPorts(sys.platform))

        
    
    def plot(self):
        
        """ Closure function for _plot to be call with parameters
            
            :returns:
                A function _plot which calls update_figure.

        """
        def _plot():
            self.update_figure()
        return _plot

    def update_figure():
        
        self.axes.clear()
        data = [] #empty array to receive data from DEM0QE

        
        self.canvas.draw() #draw

class textBox(QMainWindow):

    def __init__(self,mainWindow):
        super(textBox,self).__init__()
        self.main = mainWindow
        self.setWindowIcon(QtGui.QIcon("icon.png"))
        self.title = 'Save Plot'
        self.left = 500
        self.top = 500
        self.width = 250
        self.height = 150
        self.initUI()

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)  
        
        # Create label
        self.text = QLabel("Enter file name: ", self)
        self.text.move(20,20)
    
        # Create textbox
        self.textbox = QLineEdit(self)
        self.textbox.move(20, 50)
        self.textbox.resize(200,20)
 
        # Create a button in the window
        self.button = QPushButton('Save', self)
        self.button.move(85,90)
        self.button.resize(80,30)
        self.button.clicked.connect(self.saveFile)
 
    
        self.show()
    
    def saveFile(self):
        self.main.canvas.fig.savefig(self.textbox.text() + '.png')
        self.close()
    

class Window(QMainWindow): 
    def __init__(self):
        super().__init__()

        #Status bar 
        self.file_menu = QtWidgets.QMenu('&File', self)
        self.file_menu.addAction('&Quit', self.fileQuit,
                                 QtCore.Qt.CTRL + QtCore.Qt.Key_Q)
        self.file_menu.addAction('&Save As', self.fileSave,
                                 QtCore.Qt.CTRL + QtCore.Qt.Key_S)
        self.menuBar().addMenu(self.file_menu)
        #Help option
        self.help_menu = QtWidgets.QMenu('&Help', self)
        self.menuBar().addSeparator()
        self.menuBar().addMenu(self.help_menu)

        #create canvas object 
        self.canvas = MplCanvas(self)

        #timer configuration for refreshing graph
        self.timer = None


        

        #window config
        self.title = "Demoqe Project"
        self.top = 100
        self.left = 100
        self.width = 1000 
        self.height = 600 
        
        #push button config 
        button = QPushButton("Start", self) 
        button.move(790,350)
        button.clicked.connect(self.__plot(0))

        #push button config 
        button = QPushButton("Stop", self) 
        button.move(790,400)
        button.clicked.connect(self.stop)


        self.setWindowIcon(QtGui.QIcon("icon.png"))
        
        #slider for analog channel 1
        self.text = QLabel("Channel 1", self)
        self.text.move(800,75) 


        self.channelA2 = QSlider(Qt.Horizontal, self)
        self.channelA2.setGeometry(750,150,170,20)
        self.channelA2.setMinimum(0)
        self.channelA2.setMaximum(2)
        self.channelA2.setTickPosition(QSlider.TicksAbove)
        self.channelA2.setTickInterval(1)
        self.channelA2.valueChanged.connect(self.sliderChangedValue)

        self.chA2checkBox = QCheckBox("",self)
        self.chA2checkBox.move(950,142)
        self.chA2checkBox.resize(40,40) 

 

        self.chD1checkBox = QCheckBox("",self)
        
        self.chD1checkBox.move(950,192)
        self.chD1checkBox.resize(40,40) 

        self.chD2checkBox = QCheckBox("",self)
        
        self.chD2checkBox.move(950,242)
        self.chD2checkBox.resize(40,40) 


        self.InitWindow()

    def InitWindow(self):
        self.setWindowTitle(self.title) #show windows title
        self.setGeometry(self.top, self.left, self.width, self.height) #set window geometry
        self.show() #show all above

    def fileQuit(self): 
        self.close() #exit application
    
    def fileSave(self):
        self.newTb = textBox(self)
        self.newTb.show()
 

    
        
    
    



    def plot(selfs):
        
        """ 
            Sets timer and calls canvas plot internal function

        """
        
        if self.timer: #if timer is running stop and restart a new one
            self.timer.stop()
            self.timer.deleteLater()

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.canvas.plot())
        self.timer.start(2.5) 
        
      

    def stop(self):
        self.timer.stop() #kill timer
        

        


    
#Run GUI 

App = QApplication(sys.argv)
window = Window()
window.show()
sys.exit(App.exec())

