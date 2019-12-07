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

    p = serial.Serial(port, baudrate, timeout=None)
    return p 
        
    
def send_start_signal(port):
    request_master_operation(port)

def request_master_operation(port):
    
    msg = 0
    frame = []
    frame.append((((( b2b("1000")[0] << 4 ) | (msg >> 4)))))
    frame.append(((msg & 0x0f)))
    frame.append(((( 0 << 3 ) | 0)))
    frame.append(((( 0 << 3 ) | 0)))
    
    s = 0
    for i in range(4):
        s = s << 8 | frame[i] 

    f= b2b(bin(s))

    port.write(f)

def request_slave_operation(port):
    msg = 0
    frame = []
    frame.append((((( b2b("1100")[0] << 4 ) | (msg >> 4)))))
    frame.append(((msg & 0x0f)))
    frame.append(((( 0 << 3 ) | 0)))
    frame.append(((( 0 << 3 ) | 0)))
    
    s = 0
    for i in range(4):
        s = s << 8 | frame[i] 

    f= b2b(bin(s))

    port.write(f)

def send_master_frame(port, message,zones):
    msg = int(message)
    frame = []
    frame.append((((( b2b("1000")[0] << 4 ) | (msg >> 4)))))
    frame.append(((msg & 0x0f)))
    frame.append(((( zones[0] << 3 ) | zones[1])))
    frame.append(((( zones[2] << 3 ) | zones[3])))
    
    s = 0
    for i in range(4):
        s = s << 8 | frame[i] 

    f= b2b(bin(s))

    port.write(f)

def receive_frame(port):

    frame = port.read(4)
    
    if (frame[0] & 0x80) >> 7 == 1:
        mh = frame[0] & 0x0f
        ml = frame[1] & 0x0f
        m = (mh << 4) | ml
        return m
    else:
        return -1



def b2b(s):
    v = int(s, 2)
    b = bytearray()
    while v:
        b.append(v & 0xff)
        v >>= 8
    return bytes(b[::-1])


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
    
if __name__ == '__main__':
    port = openPort()
    masterFrameBuilder(port,"97", [1,1,1,1])
