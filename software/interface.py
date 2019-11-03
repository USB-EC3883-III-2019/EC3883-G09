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
from com import *


class MplCanvas(FigureCanvas):
    """ 
    
        Matplotlib Canvas object for creating plotting window

    """

    def __init__(self, parent=None, width=1, height=1, dpi=100):

        #Plot config
        self.fig = plt.figure()  
        FigureCanvas.__init__(self, self.fig)
        self.axes = self.fig.add_subplot(111, projection="polar")
        self.axes.set_thetamin(-30)
        self.axes.set_thetamax(210)
        #hide x's and y's labels 
        self.axes.set_xticklabels(np.linspace(0,360,10))
        self.axes.set_yticklabels([])
        

        #Grid configuration
        self.axes.set_facecolor((1,1,1))

        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(parent) #really important to do this for the gui to show image
        self.canvas.move(10,40) 
        
        self.fig.set_facecolor("none")
        self.canvas.setStyleSheet("background-color:transparent;")
        self.axes.set_ylim([0.0, 80.0])
        self.axes.set_xlim([math.pi*(-30)/180, math.pi*(210)/180])

        
        self.file = open("solindar.log", "w")
        self.file.write("Posi\tSonar\tLidar\tFusion\n" )
        #---------------------------------------------------
        # Data and serial conf 
        # --------------------------------------------------
        self.fusion = {}
        self.dataSerial = openPort()
        
        self.update_data_f = False
        self.sonar = {}
        self.lidar = {}
        
        
    
    def plot(self, channels):
        
        """ Closure function for _plot to be call with parameters
            
            :returns:
                A function _plot which calls update_figure.

        """
        def _plot():
            self.update_figure(channels)
        return _plot

    def update_figure(self, channels):
        self.axes.clear()
        
         #empty array to receive data from DEM0QE
        data = receiveData(self.dataSerial)
        self.sonar[data[0]] = data[1]
        self.lidar[data[0]] = data[2]
        
        sigma1 = 0.2619**-2
        sigma2 = 0.6476**-2
        sigma3 = 1/(sigma1 + sigma2)
        fus  = (sigma3)*(sigma1*data[1] + sigma2*data[2])
        self.fusion[data[0]] = fus


        
        self.file.write( str(round(data[0],2)) + "\t" + str(round(data[1],2)) + "\t" 
                       + str(round(data[2],2)) + "\t" + str(round(fus,2)) + "\n")

        self.axes.set_thetamin(-30)
        self.axes.set_thetamax(210)
        self.axes.set_ylim([0.0, 80.0])


        #hide x's and y's labels 
        self.axes.set_yticklabels([])
        self.axes.set_xticklabels(np.linspace(0,360,10))
        #self.axes.grid(b=True,which='major',color='r',linestyle='-')
        #self.axes.minorticks_on()
        #self.axes.grid(b=True,which='minor',linestyle='--')
        self.axes.set_facecolor((1,1,1))

        posData = list(self.sonar)
        sonarData = list(self.sonar.values())
        lidarData = list(self.lidar.values())
        sonarlidarData = list(self.fusion.values())
        if channels[0]: self.axes.scatter(posData, sonarData)
        if channels[1]: self.axes.scatter(posData, lidarData, color='r')
        if channels[2]: self.axes.scatter(posData, sonarlidarData, color='y' )
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
        self.title = "SONAR/LIDAR"
        self.top = 100
        self.left = 100
        self.width = 800 
        self.height = 600 
        

        self.channels = [False,False,False]
        #push button config 
        button = QPushButton("Start", self) 
        button.move(600,200)
        button.clicked.connect(self.plot)

        button = QPushButton("Stop", self) 
        button.move(600,250)
        button.clicked.connect(self.stop)

        button = QPushButton("LIDAR", self) 
        button.move(600,300)
        button.clicked.connect(self.plot_lidar)

        button = QPushButton("SONAR", self) 
        button.move(600,350)
        button.clicked.connect(self.plot_sonar)
        
        button = QPushButton("SOLINDAR", self) 
        button.move(600,400)
        button.clicked.connect(self.plot_solindar)

        #push button config 
        


        self.setWindowIcon(QtGui.QIcon("icon.png"))



        self.InitWindow()

    def plot_sonar(self):
        self.channels[0] = not self.channels[0]
        def __plot():
            self.canvas.plot(self.channels)

        return __plot

    def plot_lidar(self):
        self.channels[1] =  not self.channels[1]

        def __plot():
            
            self.canvas.plot(self.channels)

        return __plot
    
    def plot_solindar(self):
        self.channels[2] = not self.channels[2]
        def __plot():
            self.canvas.plot(self.channels)

        return __plot


    def InitWindow(self):
        self.setWindowTitle(self.title) #show windows title
        self.setGeometry(self.top, self.left, self.width, self.height) #set window geometry
        self.show() #show all above

    def fileQuit(self): 
        self.close() #exit application
    
    def fileSave(self):
        self.newTb = textBox(self)
        self.newTb.show()
 

    def plot(self):
        
        """ 
            Sets timer and calls canvas plot internal function

        """
        
        if self.timer: #if timer is running stop and restart a new one
            self.timer.stop()
            self.timer.deleteLater()

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.canvas.plot(self.channels))
        self.timer.start(16) 
        
      

    def stop(self):
        self.timer.stop() #kill timer
        

        


    
#Run GUI 

App = QApplication(sys.argv)
window = Window()
window.show()
sys.exit(App.exec())

