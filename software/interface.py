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

        self.master = QRadioButton("Master", self)
        self.master.move(100,50)
        self.master.setChecked(True)
        #self.master.toggled.connect(lambda:self.master_slave(self.master))
        #self.beginOperation()


        self.slave = QRadioButton("Slave", self)
        self.slave.move(250,50)
        self.slave.setEnabled(True)
        self.slave.toggled.connect(lambda:self.master_slave(self.slave))

        self.label = QLabel("Torre 1", self)
        self.label.move(45,100)
        self.t1 = QComboBox(self)
        self.t1.addItems(["z1","z2","z3", "z4", "z5", "z6"])
        self.t1.adjustSize()
        self.t1.move(100,100)


        self.label = QLabel("Torre 2", self)
        self.label.move(45,150)
        self.t2 = QComboBox(self)
        self.t2.addItems(["z1","z2","z3", "z4", "z5", "z6"])
        self.t2.adjustSize()
        self.t2.move(100,150)

        self.label = QLabel("Torre 3", self)
        self.label.move(45,200)
        self.t3 = QComboBox(self)
        self.t3.addItems(["z1","z2","z3", "z4", "z5", "z6"])
        self.t3.adjustSize()
        self.t3.move(100,200)
        
        self.label = QLabel("Torre 4", self)
        self.label.move(45,250)
        self.t4 = QComboBox(self)
        self.t4.addItems(["z1","z2","z3", "z4", "z5", "z6"])
        self.t4.adjustSize()
        self.t4.move(100,250)
        
        self.sendText = QPlainTextEdit(self)
        self.sendText.resize(40,30)
        self.sendText.move(100,300)

        self.send = QPushButton("Enviar",self)
        self.send.move(50,300)
        self.send.resize(45,30)
        self.send.clicked.connect(lambda:self.send_btn())

        self.label = QLabel("Recibido", self)
        self.label.move(50,350)

        self.message = QPlainTextEdit(self)
        self.message.resize(40,30)
        self.message.move(100,350)


        self.label = QLabel("Recibido", self)
        self.label.move(200,100)

        self.received = QPlainTextEdit(self)
        self.received.resize(40,30)
        self.received.move(260,100)
        self.received.setReadOnly(True)

        self.port = openPort()

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
 
    # def plot(self):
        
    #     """ 
    #         Sets timer and calls canvas plot internal function

    #     """
        
    #     if self.timer: #if timer is running stop and restart a new one
    #         self.timer.stop()
    #         self.timer.deleteLater()

    #     self.timer = QtCore.QTimer(self)
    #     self.timer.timeout.connect(self.canvas.plot(self.channels))
    #     self.timer.start(16)

    def beginOperation(self):
        send_start_signal(self.port)

    def send_btn(self):
        z1 = self.t1.currentIndex()+1
        z2 = self.t2.currentIndex()+1
        z3 = self.t3.currentIndex()+1
        z4 = self.t4.currentIndex()+1
        print(self.sendText.document().toPlainText())
        send_master_frame(self.port,self.sendText.document().toPlainText(),[z1,z2,z3,z4])
        receiving_data = receive_frame(self.port)
        print(receiving_data) 
        self.message.setPlainText(receiving_data)
    

    # def master_slave(self, btn):
    #     if btn.text() == "Slave":
    #         request_slave_operation(self.port)

    #     while btn.text() == "Slave":
    #         self.send.setEnabled(False)
    #         data = receive_frame(self.port)
    #         print(data)
    #         self.received.insertPlainText(data)
    #         print("Me estoy muriendo")

    #     if btn.text() == "Master":
    #         #request_master_operation(self.port)
    #         self.send.setEnabled(True)
    #         #data = receive_frame(self.port)
    #         #print(data)

            
        

    def stop(self):
        self.timer.stop() #kill timer
        

        


    
#Run GUI 

App = QApplication(sys.argv)
window = Window()
window.show()
sys.exit(App.exec())

