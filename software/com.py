#!python 3.x
#################################################
## Author       : Jose Moran
## Github       : jose0796
## email        : jmoran071996@gmail.com
## file         : com.py 
## Description  : Library for digital and analog 
## unframing and processing for MC9S08QE128 uC
#################################################



def openPort(port="/dev/ttyUSB0",baudrate=115200):

    import serial 

    p = serial.Serial(port, baudrate)
    return p 
        

def synchronize(dataSerial):
    """ Synchronize data received from serial port
    """
    while 1:
        f = dataSerial.read()
        if f[0] & 0x80 == 0:
            for _ in range(3):
                dataSerial.read()
            break
        



def receiveData(dataSerial):
    f = dataSerial.read()

    pos = -1
    s = -1
    l = -1


    if f[7] & 0x80 == 0: 
        #synchronized already
        pos = unframeData(f)
        f = dataSerial.read()
        s = f << 2
        f = dataSerial.read()
        s = s | ((f & 0x60) >> 5)
        l = (f & 0x1f) << 7
        f = dataSerial.read()
        l = l | (f & 0x7f)

        print(pos,s,l)

    else:
        synchronize(dataSerial)
    


    

        
    

    
    

    


