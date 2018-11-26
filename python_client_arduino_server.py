# Client for Arduino based MAX31856 Six Channel Temp Reader

import serial
import time
import csv
import datetime

temps = []
times = []

#arduino = serial.Serial('/dev/cu.usbmodem1441',115200) #connecting to serial port - top usbC connection on right side of comp
arduino = serial.Serial('COM3', 9600)
time.sleep(2)

filename = input('What would you like to name this file (file path) ')
delay = input('How many seconds delay would you like between temperature readings ')

arduino.write(str.encode("V"))
for i in range(9):
    response = arduino.readline()[:-2]
    if response:
        print(response.decode())

while True:
    text = input("Command: ")
    arduino.write(str.encode(text))
    if text.upper() != 'T*':
        response = arduino.readline()[:-2]
        if response:
            print(datetime.datetime.now(), response.decode())
    else:
        while True:
            for i in range(0,6): #this is so that it reads all 6 temperatures at one time point
                x = arduino.readline()
                temps.append(x.decode())
                date= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S') #getting current datetime from computer
                times.append(date)
                print (x.decode(), date)
    
            time.sleep(int(delay)) #tells it how many seconds to wait before sampling temperatures again
            
            values = []
            newtemps = []
            newnum = []
    
            for i in range(len(temps)): #splitting temperature output by probe # and actual temp & saving to separate vars
                values.append(temps[i].split())
                newtemps.append(values[i][1])
                newnum.append(values[i][0])
                
    
            myFile = open(filename, 'w')  #creating a csv based on filename and w for write to file
    
            with myFile:  #writing to csv
                myFields = ['Datetime', 'ProbeNumber','Temps']
                writer = csv.DictWriter(myFile, fieldnames=myFields)    
                writer.writeheader()
                for j,k,l in zip(times,newnum,newtemps):
                    writer.writerow({'Datetime' : j, 'ProbeNumber': k, 'Temps': l})
            arduino.write(str.encode(text))
