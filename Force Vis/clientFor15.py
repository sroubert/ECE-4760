# 15 Data points test.

import bluetooth
import csv
import time
import sys

def writeToFile( data1 ):

	#path1 = r"C:\Users\Jonah\OneDrive - Cornell University\Fall 2017\ECE 4760\Final Project\Server\ "
	try:
		with open('data15.csv', 'w',  newline='') as f:

			writer = csv.writer(f, delimiter=',')
			writer.writerow(data1) 
	except:
		print("Data not written")

# Mac address of the HC-06:
macAddress = '98:d3:31:fd:31:27'
port = 1 

# Startup bluetooth connection:
s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
s.connect((macAddress,port))

decodedData = ""

try:

	while True:
		time.sleep(0.1)
		data = s.recv(1024)
		#print( "Data:  " + str(data))
		val = len(data)
		if (data[val-1] == 10):

			decodedData = decodedData + data[0:val-1].decode('utf-8', "ignore")
			print( "Decoded Data: " + str(decodedData) )
			rayData = str(decodedData).split()
			#print( "Data: ", str(decodedData).split())

			"""
			for i in range(len(rayData)):
				decodedData = decodedData
				#l = 
				#p = rayData[0][0]
				#num = rayData[0][]
			"""

			decodedData = ""

		else:

			decodedData = decodedData + data.decode('utf-8', "ignore")
		#print("Data", data.decode('utf-8', "ignore"))

except Exception as e:
	print( "Exception:  " + str(e))
	pass

print("Disconnected")

s.close()