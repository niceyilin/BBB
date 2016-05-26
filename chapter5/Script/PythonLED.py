#!/usr/bin/python
import sys
LED_PATH="/sys/class/leds/beaglebone:green:usr3";


def writeLedFile(filename, value, path=LED_PATH):
    "this function write value to a LED related file"
    try:
        fp = open(path+filename, "w")
    except IOError:
        print "can't open file", LED_PATH+filename
        sys.exit(3)
    fp.write(value)
    fp.close()
    return

def readLedFile(filename, path=LED_PATH):
    "this function read out and return a LED related file"
    try:
        fp = open(path+filename, "r")
    except IOError:
        print "can't open file", LED_PATH+filename
        sys.exit(3)
    print fp.read()
    fp.close()
    return
    
#sys.argv[0] is ./PythonLED.py
if len(sys.argv) != 2:
    print("Error : wrong usage\n")
    print("./BashLED.sh followed by one of these cmd...\n-on\n-off\n-flash\n-status\n")
    print("e.g. ./PythonLED.py on led3\n")
    sys.exit(2)
elif sys.argv[1] == "on":
    print "turn LED on"
    writeLedFile(filename="/trigger", value="none")
    writeLedFile(filename="/brightness", value="1")
elif sys.argv[1] == "off":
    print "turn LED off"
    writeLedFile(filename="/trigger", value="none")
    writeLedFile(filename="/brightness", value="0")    
elif sys.argv[1] == "flash":
    print "flash LED "
    writeLedFile(filename="/trigger", value="timer")
    writeLedFile(filename="/delay_on", value="50")
    writeLedFile(filename="/delay_off", value="50")
elif sys.argv[1] == "status":
    print "return LED status"
    readLedFile(filename="/trigger")
elif sys.argv[1] == "dim":
    print "make 4 LEDs dimmer and dimer"
    writeLedFile(filename="/trigger", value="timer", path="/sys/class/leds/beaglebone:green:usr3")
    writeLedFile(filename="/delay_on", value="1", path="/sys/class/leds/beaglebone:green:usr3")
    writeLedFile(filename="/delay_off", value="20", path="/sys/class/leds/beaglebone:green:usr3")
    
    writeLedFile(filename="/trigger", value="timer", path="/sys/class/leds/beaglebone:green:usr2")
    writeLedFile(filename="/delay_on", value="3", path="/sys/class/leds/beaglebone:green:usr2")
    writeLedFile(filename="/delay_off", value="7", path="/sys/class/leds/beaglebone:green:usr2")
    
    writeLedFile(filename="/trigger", value="timer", path="/sys/class/leds/beaglebone:green:usr1")
    writeLedFile(filename="/delay_on", value="6", path="/sys/class/leds/beaglebone:green:usr1")
    writeLedFile(filename="/delay_off", value="4", path="/sys/class/leds/beaglebone:green:usr1")
    
    writeLedFile(filename="/trigger", value="timer", path="/sys/class/leds/beaglebone:green:usr0")
    writeLedFile(filename="/delay_on", value="9", path="/sys/class/leds/beaglebone:green:usr0")
    writeLedFile(filename="/delay_off", value="1", path="/sys/class/leds/beaglebone:green:usr0")
else:
    print "Error: Invalid commend!"
    sys.exit(2)
    
print "Job is done."


    