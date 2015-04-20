import requests
import serial

#Rename COM13 to the port where your arduino is plugged in
ser = serial.Serial('COM13', 9600)

while True:
    r = requests.get('http://oculuscar.herokuapp.com/')
    print r.text
 
    for letter in r.text:
        print str(letter)
        ser.write(str(letter))
    ser.write('\n')