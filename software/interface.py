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

        
        #timer configuration for refreshing graph
        self.timer = None

        #window config
        self.title = "PROYECTO 2"
        self.top = 100
        self.left = 100
        self.width = 400 
        self.height = 400 

        master = QRadioButton("Master", self)
        master.move(100,100)
        master.setChecked(True)

        slave = QRadioButton("Slave", self)
        slave.move(250,100)
        slave.setEnabled(True)


        label = QLabel("Torre 1", self)
        label.move(50,150)
        t1 = QComboBox(self)
        t1.addItem("z1")
        t1.addItem("z2")
        t1.addItem("z3")
        t1.adjustSize()
        t1.move(100,150)


        label = QLabel("Torre 2", self)
        label.move(50,200)
        t2 = QComboBox(self)
        t2.addItem("z1")
        t2.addItem("z2")
        t2.addItem("z3")
        t2.adjustSize()
        t2.move(100,200)

        label = QLabel("Torre 1", self)
        label.move(50,250)
        t3 = QComboBox(self)
        t3.addItem("z1")
        t3.addItem("z2")
        t3.addItem("z3")
        t3.adjustSize()
        t3.move(100,250)
        
        sendText = QPlainTextEdit(self)
        sendText.resize(40,30)
        sendText.move(100,300)

        send = QPushButton("Enviar",self)
        send.move(50,300)
        send.resize(45,30)

        label = QLabel("Recibido", self)
        label.move(200,150)

        received = QPlainTextEdit(self)
        received.resize(40,30)
        received.move(260,150)


        self.setWindowIcon(QtGui.QIcon("icon.png"))
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

