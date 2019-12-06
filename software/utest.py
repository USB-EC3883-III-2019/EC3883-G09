def masterFrameBuilder(message,zones):
    msg = int(message)
    frame = []
    frame.append((((( b2b("1000")[0] << 4 ) | (msg >> 4)))))
    frame.append(((msg & 0x0f)))
    frame.append(((( zones[0] << 3 ) | zones[1])))
    frame.append(((( zones[2] << 3 ) | zones[3])))
    s = 0
    for i in range(4):
        s = s << 8 | frame[i] 

    return b2b(bin(s))

def b2b(s):
    v = int(s, 2)
    b = bytearray()
    while v:
        b.append(v & 0xff)
        v >>= 8
    return bytes(b[::-1])

print(masterFrameBuilder("97",[1,1,1,1]))