import serial
import csv
import time
import sys

def main():
	try:
		me, input_devicename, output_filename = sys.argv
	except:
		print "Usage: {} <serial device> <output filename>".format(sys.argv[0])
		return 1

	if not input_devicename.startswith("/dev/"):
		input_devicename = "/dev/" + input_devicename
		
	s = serial.Serial(input_devicename, 115200)
	f = file(output_filename,'at+')

	try:
		while True:
			ping = s.readline()
			now = time.time()
			f.write("{}\n".format(now))
			print "Ping {}".format(now)

	except KeyboardInterrupt: pass

	finally:
		f.close()

if __name__ == "__main__":
	sys.exit(main())