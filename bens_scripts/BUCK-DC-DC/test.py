#!/usr/bin/env python

import random

import minimalmodbus

minimalmodbus.BAUDRATE = 9600
minimalmodbus.TIMEOUT = 0.5  # must be low latency somehow??
minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = False


def decode_response(b):
    if len(b) != 20:
        print (len(b))
        return

    rv = {'U-Set': b[0] / 100.0,
          'I-Set': b[1] / 1000.0,
          'U-Out': b[2] / 100.0,
          'I-Out': b[3] / 1000.0,
          'P-Out': b[4] / 100.0,
          'U-In': b[5] / 100.0,
          'Locked': {0: 'OFF', 1: 'ON'}.get(b[6]),
          'Protected': {0: 'ON', 1: 'OFF'}.get(b[7]),
          'CV/CC': {0: 'CV', 1: 'CC'}.get(b[8]),
          'ON_OFF': {0: 'OFF', 1: 'ON'}.get(b[9]),
          'Backlight': b[10],
          'Model': str(b[11]),
          'Firmware': str(b[12] / 10.0),
          }
    return rv


if __name__ == "__main__":

    instrument = minimalmodbus.Instrument(port='/dev/ttyUSB0', slaveaddress=1)  # port name, slave address (in decimal)
    # instrument.serial.baudrate = 9600
    instrument.debug = False  # True

    for i in range(1000):
        try:

            temp = instrument.read_registers(registeraddress=0x0,
                                             numberOfRegisters=20)

            print (decode_response(temp))

            v = random.random() * 25.0  # between 0 and 25 volt output setting
            i = random.random() * 5.0  # between 0 and 5 amp output setting

            print ("set to {v:2} V {i:2} A".format(v=v, i=i))
            instrument.write_register(registeraddress=0, value=v, numberOfDecimals=2) # set V
            instrument.write_register(registeraddress=1, value=i, numberOfDecimals=3) # set I

            # set Limit Current and Voltage protection values
            instrument.write_register(registeraddress=0x52, value=v, numberOfDecimals=2)  # set V
            instrument.write_register(registeraddress=0x53, value=i, numberOfDecimals=3)  # set I

        except (IOError, ValueError) as e:
            print (e)
