#
#   Created by: Markus T Peterson
#   Date: 1/27/2020
#   Assignment 0
#   E510 - ENGINEERING DISTRIBUTED SYSTMS
#

import socket
import sys

save = {}   #Save Key value pairs

def savePair(string):   #function to save pairs
    kvArray = string.split("=") #seperates the key from value
    save[kvArray[0]] = kvArray[1] #saves the info

serv = 1
def start():
    #Next three lines for starting the server and assigning a port
        #and a host (localhost)
    serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serv.bind(('0.0.0.0', 8080))
    serv.listen(3)
    while True:
        conn, addr = serv.accept()
        from_client = ''
        while True:
            data = conn.recv(1024)  #gets client response
            if not data: break
            if len(data) > 0:
                string = data.split(" ")    #Seperates the command from info
                if string[0] == "STORE":    #checks if given STORE command
                    savePair(string[1])     #Save the information
                    conn.send("Saved")      #Send save validation
                elif string[0] == 'GET':    #checks if given GET command
                    if string[1] in save:   #checks if key exists
                        conn.send(save[string[1]])  #if yes send value
                    else:                   #if not
                        conn.send("ERROR")  #send "ERROR"
        conn.close()                        #close connection

if len(sys.argv) > 0:   #used to check if file ran from command line
    start() #starts up the server
