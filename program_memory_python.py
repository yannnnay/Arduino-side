#! /usr/bin/env python3

from serial import *
import time

Arduino = Serial(port="/dev/ttyACM1", baudrate=115200)


    
def cmd( arg):
    if len(arg) > 32:
        print("COMMAND TOO LONG")
    else:
        V = arg.encode()
        V = b'$'  + V  + b';'
        Arduino.write(V)
        Arduino.flush()
        resp = Arduino.readline()
        print(resp)
        



def reset():
    # unsetting DTR will reset the board, even on Linux
    Arduino.setDTR(False)
    Arduino.reset_input_buffer();
    Arduino.setDTR(True)

    # After reset we should chek for the reset message
    # resp = Arduino.readline().strip()
    # if b"OK: Panc'Art" == resp :
    #     print("** RESET ok: ", resp.strip())
    # else:
    #     print("** RESET not ok: ", resp.strip())
        
user_input = "rien"       
Arduino.reset_input_buffer()
reset()
while(user_input != "stop"):
    user_input = input("Enter a command: \n")
    cmd(user_input)

    
print("bye.....")
Arduino.close()
