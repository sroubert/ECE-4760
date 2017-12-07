# 15 Data points test.

import bluetooth
import csv
import time
import sys
import json

def writeToFile( data1 ):

	try:
		with open('data15.csv', 'w',  newline='') as f:

			writer = csv.writer(f, delimiter=',')
			writer.writerow(data1) 
	except:
		print("Data not written")

def writeToFile1( data1 ):

	while True:
		try:
			with open('data15.txt', 'r+' ) as f:

				f.seek(0,0)
				f.truncate()
				json.dump( data1, f)
			break
		except:
			break
			print( "Trying again..")

# Mac address of the HC-06:
macAddress = '98:d3:31:fd:31:27'
port = 1 

# Startup bluetooth connection:
print("Attempting to connect...")
s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
s.connect((macAddress,port))
print("Connected!" )
time.sleep(0.5)

# Create the json data variable
decodedData = ""
jsonData = {}
jsonData['sensorData'] = []

try:

	while True:
		# Delay for 20 ms
		time.sleep(0.02)

		# This receives the data that is sent from the PIC
		data = s.recv(1024)

		# This is to check if the end of the transmission contains the end line character
		val = len(data)
		if (data[val-1] == 10):

			# We now have the data for all fourteen sensors and can now decode then split them up into 14 individual readings
			decodedData = decodedData + data[0:val-1].decode('utf-8', "ignore")
			rayData = str(decodedData).split()

			# This for loop parses the data into a json object
			for i in range(14):
				decodedData = decodedData
				num = rayData[i][:]
				jsonData['sensorData'].append({'Sensor' : 'Sensor' + str(i+1),  'R' : int(num)})

			# This function writes the data to the json file data15.txt
			writeToFile1(jsonData)
			sys.stdout.flush()

			print( "Json Data:" , jsonData)
			print("\n")
			decodedData = ""
			jsonData = {}
			json.dumps(jsonData)
			jsonData['sensorData'] = []
			time.sleep(0.05)

		else:

			decodedData = decodedData + data.decode('utf-8', "ignore")

except Exception as e:
	print( "Exception:  " + str(e))
	pass

print("Disconnected")

s.close()