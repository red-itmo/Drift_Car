#!/usr/bin/env python

##   drift_car_serial.py
##   Created on: 03.03.2018
##        Email: Nicko_Dema@protonmail.com
##               ITMO University

import serial, re
import rospy

class serial_transceiver():
    def __init__(self):
        self.port_name = rospy.get_param('port', '/dev/ttyACM0')
        self.port_is_open = False
        self.time = 0
        self.err = 0
        self.ticks_l = 0
        self.ticks_r = 0

    #-----------------------------------------------------------------
    def open_port(self):
        try:
            self.port_is_open = False
            print("[car]: Trying to open UART port: " + self.port_name)
            self.port = serial.Serial(  port=self.port_name,
                                        baudrate=115200,
                                        parity=serial.PARITY_NONE,
                                        stopbits=serial.STOPBITS_ONE,
                                        bytesize=serial.EIGHTBITS)
            self.port_is_open = True
            self.port.flushInput()
            self.port.flushOutput()
            return True
        except serial.SerialException:
            print("[car]: ERROR | failed to open UART port")
            return False

    #-----------------------------------------------------------------
    def set_cmd(self, vel_l, vel_r):
        if (self.port.isOpen()):
            try:
                self.port.write("<" + vel_l + " " + vel_r + ">\n")
            except:
                self.port.close()
                self.port.open()
    #-----------------------------------------------------------------
    def get_enc_data(self):
        if (self.port.readable()):
            self.enc_data_str = ""
            try:
                while True:
                    in_ch = self.port.read()
                    #print in_ch
                    if in_ch == '\n':
                        break
                    self.enc_data_str += in_ch

                serObj = re.search(r'<(-?[0-9]+)\s(-?[0-9]+)\s(-?[0-9]+)\s(-?[0-9]+)>', self.enc_data_str, re.I)
                if (serObj):
                    self.ticks_l = int(serObj.group(1))
                    self.ticks_r = int(serObj.group(2))
                    self.time = int(serObj.group(3))
                    self.err = int(serObj.group(4))
                    return True
                else:
                    print("[car]: Received data doesn't belong to encoder's ticks format: ")
                    print self.enc_data_str
                    return False
            except:
                print("[car]: Port reading ERROR")
                return False
        else:
            print("[car]: Port is empty to reading")
            return False

    #-----------------------------------------------------------------
    def is_open(self):
        return self.port.isOpen()

    def __del__(self):
        self.port_is_open = False
        try:
            self.port.close()
        except AttributeError:
            print("[car]: Port closing ERROR, no port was created")

def seyhello ():
    print "hello?"
