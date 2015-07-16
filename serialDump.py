import serial
import csv
import time

arduino = serial.Serial('/dev/cu.wchusbserial1410', 9600, timeout=.1)
current_time = time.strftime("%I:%M:%S");

while True:
	with open('led-time.csv', 'wb') as csvfile:
	        spamwriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
	        spamwriter.writerow([arduino.readline()[:-2], current_time])
	        lastTick = [arduino.readline()[:-2]