from xbee import XBee
import serial
import time

ser = serial.Serial('/dev/tty.usbserial-A8004x6O', 57600)

xbee = XBee(ser)

while True:
	xbee.remote_at(dest_addr='\x00\x02', command='D3', parameter='\x05')
	time.sleep(0.1)
	xbee.remote_at(dest_addr='\x00\x02', command='D3', parameter='\x04')
	time.sleep(0.1)

xbee.remote_at(dest_addr='\x00\x02', command='D3', parameter='\x04')

ser.close()