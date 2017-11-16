import bluetooth
import csv
import time

def writeToFile( data1 ):

	path1 = r"C:\Users\Jonah\OneDrive - Cornell University\Fall 2017\ECE 4760\Final Project\Server\ "
	with open( path1 + 'data.csv', 'w',  newline='') as f:

		writer = csv.writer(f, delimiter=',')
		writer.writerow(data1) 

# Mac address of the HC-06:
macAddress = '98:d3:31:fd:31:27'
port = 1 

# Startup bluetooth connection:
s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
s.connect((macAddress,port))


try:

	while True:

		time.sleep(0.2)
		data = s.recv(1024)

		if len(data) == 0: break

		val = len(data)
		if ( data[val-1] == 10 ):
			decodedData = decodedData + data.decode('utf-8', "ignore")
			print( p, "Received: ", int(decodedData))
			writeToFile([int(decodedData)])
		else:

			if (data[0] == 97):

				p = 'A'

			elif (data[0] == 98):

				p = 'B'

			elif (data[0] == 99):

				p = 'C'

			else:

				p = 'D'

			decodedData = data[0:val-1].decode('utf-8', "ignore")

		#print("Beginning: ", data[0])

except Exception as e:
	print( "Error was thrown:   " + str(e) )
	pass

print("Disconnected")

s.close()