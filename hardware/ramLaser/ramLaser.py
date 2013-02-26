#!/usr/bin/python

from xbee import XBee
import serial
import time

from simpleOSC import initOSCClient, initOSCServer, setOSCHandler, sendOSCMsg, closeOSC, \
     createOSCBundle, sendOSCBundle, startOSCServer

ser = serial.Serial('/dev/tty.usbserial-A8004x6O', 57600)

xbee = XBee(ser)

def LaserLoop():
	initOSCClient('127.0.0.1', 8000) # takes args : ip, port
	initOSCServer('127.0.0.1', 8001) # takes args : ip, port, mode --> 0 for basic server, 1 for threading server, 2 for forking 

	# bind addresses to functions 
	setOSCHandler('/laser', laser)
	startOSCServer() # and now set it into action

	print 'ready to receive and send osc messages ...'

def laser(addr, tags, data, source):
	if data[0] == 1:
		xbee.remote_at(dest_addr='\x00\x02', command='D3', parameter='\x05')
	    #print 'on'
	elif data[0] == 0:
		xbee.remote_at(dest_addr='\x00\x02', command='D3', parameter='\x04')
		#print 'off'
	
if __name__ == '__main__': LaserLoop()