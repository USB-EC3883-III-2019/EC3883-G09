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
import threading
import ctypes
import time






class Window(QMainWindow): 
    def __init__(self):
        super().__init__()

        
        #timer configuration for refreshing graph
        self.timer = None

        #window config
        self.title = "PROYECTO 2"
        self.top = 100
        self.left = 100
        self.width = 400 
        self.height = 400 

        self.port = openPort()
        self.port.flush()

        self.label = QLabel("Recibido", self)
        self.label.move(200,100)

        self.received = QPlainTextEdit(self)
        self.received.resize(40,30)
        self.received.move(260,100)
        self.received.setReadOnly(True)

        self.master = QRadioButton("Master", self)
        self.master.move(100,50)
        self.master.setChecked(True)
        self.master_exec = None
        #self.master.toggled.connect(lambda:self.master_slave(self.master))
        #self.beginOperation()


        self.slave = QRadioButton("Slave", self)
        self.slave.move(250,50)
        self.slave.setEnabled(True)
        self.slave.toggled.connect(lambda:self.start_receiving(self.slave))
        self.slave_exec = self.slave_thread(self.slave,self.port)

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


        

        

        self.setWindowIcon(QtGui.QIcon("icon.png"))
        self.InitWindow()

    def InitWindow(self):
        self.setWindowTitle(self.title) #show windows title
        self.setGeometry(self.top, self.left, self.width, self.height) #set window geometry
        self.show() #show all above


    class master_thread(threading.Thread):

        def __init__(self, port):
            threading.Thread.__init__(self)
            self.port = port

        def run(self):
            data = receive_frame(self.port)
            print("Recibiendo Master: " + data)
            #self.btn.setPlainText(data)

        def get_id(self):
            if hasattr(self, '_thread_id'):
                return self._thread_id
            for id, thread in threading._active.items():
                if thread is self:
                    return id

        def __stop():
            thread_id = self.get_id()
            res = ctypes.pythonapi.PyThreadState_SetAsycExc(thread_id,ctypes.py_object(SystemExit))
            if res > 1:
                ctypes.pythonapi.PyThreadState_SetAsycExc(thread_id,0)
                print("Aborted Master thread")

    class slave_thread(threading.Thread):

        def __init__(self, btn, port):
            threading.Thread.__init__(self)
            self.btn = btn
            self.port = port

        def run(self):
            while self.btn.isChecked(): 
                print("Estoy corriendito papa")
                data = receive_frame(self.port)
                print("Recibiendo Slave: " + data)

            print("Saliendo papa")
            self.stop()
        
        def get_id(self):
            if hasattr(self, '_thread_id'):
                return self._thread_id
            for id, thread in threading._active.items():
                if thread is self:
                    return id

        def stop(self):
            thread_id = self.get_id()
            res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id,ctypes.py_object(SystemExit))
            if res > 1:
                ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id,0)
                print("Aborted Slave thread")

    def send_btn(self):
        # if self.slave_exec.isAlive():
        #     self.slave_exec.stop()
        #     self.slave_exec = self.slave_thread(self.slave,self.port)
        z1 = self.t1.currentIndex()+1
        z2 = self.t2.currentIndex()+1
        z3 = self.t3.currentIndex()+1
        z4 = self.t4.currentIndex()+1
        print("Enviando: " + self.sendText.document().toPlainText())
        send_master_frame(self.port,self.sendText.document().toPlainText(),[z1,z2,z3,z4])
        self.master_exec = self.master_thread(self.port)
        self.master_exec.start()
        #self.master_exec.join()


    def start_receiving(self, btn):
        # if self.slave_exec.isAlive():
        #     self.slave_exec.stop()
        #     self.slave_exec = self.slave_thread(self.slave,self.port)
        request_slave_operation(self.port)
        self.slave_exec.start()
        #self.slave_exec.join()
        

    def stop(self):
        self.timer.stop() #kill timer
        

        


    
#Run GUI 

App = QApplication(sys.argv)
window = Window()
window.show()
sys.exit(App.exec())

