#!python 3.x
#################################################
## Author       : Jose Moran
## Github       : jose0796
## email        : jmoran071996@gmail.com
## file         : com.py 
## Description  : Library for digital and analog 
## unframing and processing for MC9S08QE128 uC
#################################################



def openPort(port="COM6",baudrate=115200):

    import serial 

    p = serial.Serial(port, baudrate,timeout=10)
    return p 
        

def synchronize(dataSerial):
    """ Synchronize data received from serial port
    """
    while 1:
        f = dataSerial.read(1)
        if f[0] & 0x80 == 0:
            dataSerial.read(1)
            break
        



def receiveData(dataSerial):
    
    import math
    f = dataSerial.read(4)

    pos = -1
    s = -1
    l = -1


    if f[0] & 0x80 == 0: 
            #synchronized already
        pos = f[0]
        s = (((f[1] & 0x7f) << 2) & 0x3fc) | ((f[2] & 0x60) >> 5) 
        l = (f[2] & 0x1f) << 7 | (f[3] & 0x7f)
        raw_pos = pos
        pos = math.pi*((360/96)*(pos) - 30)/180
        s *= 61.035156/58
        s = min(s,80)
        #l  = (l*3)/(4096*0.6)
        
        l = convertLidar(l)
        l = min(l,80)
        

    else:
        synchronize(dataSerial)

    return (pos,s,l)
    
def convertLidar(x):
    #return 21.734*(x**4) - 138.92*(x**3) + 330.98*(x**2) - 361.52*(x) + 173.81
    
    #import math 
    #return 109750*(x)**(-1.23)     
    return 24067*(x**(-1.011))      #Best option
    
