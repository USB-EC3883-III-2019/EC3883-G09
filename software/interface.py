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

        self.title = "Proyecto 2"
        self.top = 100
        self.left = 100
        self.width = 400
        self.height = 350
        

        self.channels = [False,False,False]
        #push button config 
        button = QRadioButton("Master",self) 
        button.move(100,50)
        button.setChecked(True)
        #button.clicked.connect(self.plot)

        button = QRadioButton("Slave", self) 
        button.move(250,50)
        #button.clicked.connect(self.stop)

        


        self.t1Tag = QLabel("Torre 1", self)
        self.t1Tag.move(70,95)

        torre1 = QComboBox(self)
        torre1.addItem("z1")
        torre1.addItem("z2")
        torre1.addItem("z3")
        torre1.move(120,100)
        torre1.resize(40,25)

        self.t2Tag = QLabel("Torre 2", self)
        self.t2Tag.move(70,145)
        torre2 = QComboBox(self)
        torre2.addItem("z1")
        torre2.addItem("z2")
        torre2.addItem("z3")
        torre2.move(120, 150)
        torre2.resize(40,25)


        self.t3Tag = QLabel("Torre 3", self)
        self.t3Tag.move(70,195)
        torre3 = QComboBox(self)
        torre3.addItem("z1")
        torre3.addItem("z2")
        torre3.addItem("z3")
        torre3.move(120, 200)
        torre3.resize(40,25)

        self.t1Tag = QPushButton("Enviar", self)
        self.t1Tag.resize(50,25)
        self.t1Tag.move(65,250)
        textArea = QPlainTextEdit(self)
        textArea.resize(40,25)
        textArea.move(120,250)
        
        self.t1Tag = QLabel("Recibido", self)
        self.t1Tag.move(200,95)
        textArea = QPlainTextEdit(self)
        textArea.resize(40,25)
        textArea.move(260,100)

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

