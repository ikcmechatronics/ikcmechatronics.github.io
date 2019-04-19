import time
from smbus2 import SMBus, i2c_msg, SMBusWrapper  # import smbus2 library

# String To Byte Function
def StringToBytes(val):
    retVal = []
    for c in val:
        retVal.append(ord(c))
    return retVal

inComingMsg = ""
# Infinite Loop
while(1):
    dataToSend = "*250,100#"    # Data Package to be sent. This package can be created dynamically.
    # This is the part where i2c reading and writing occurs;
    with SMBusWrapper(1) as bus:
        # Read 10 bytes from address 0x20
        msg = i2c_msg.read(0x20, 10)
        bus.i2c_rdwr(msg)
        # Write dataToSend package (as Bytes) to address 0x20
        sendMsg = i2c_msg.write(0x20,StringToBytes(dataToSend))
        bus.i2c_rdwr(sendMsg)
        # Alternatively, write integer array to address 0x20
        #sendMsg = i2c_msg.write(0x20, [65, 66, 67, 68])
        #bus.i2c_rdwr(sendMsg)
    data = list(msg)  # incoming message (msg) is transformed to data list.
    # Create string from incoming message (msg) => inComingMsg (string)
    for k in range(msg.len):
        inComingMsg = inComingMsg + msg.buf[k]
    print(inComingMsg)  # Printing all incoming message on the screen. (This information can be seperated to be used later)
    inComingMsg = ""    # Deleting inComingMsg
    time.sleep(0.1)
