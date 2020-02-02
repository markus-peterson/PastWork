#
#   Created by: Markus T Peterson
#   Date: 1/27/2020
#   Assignment 0
#   E510 - ENGINEERING DISTRIBUTED SYSTMS
#

import socket
import sys

save = [] #Used to save all keys in string form

#Next two lines for connecting to the server socket
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('0.0.0.0', 8080))

if len(sys.argv) > 1:   #Used for command line arguments
    key = ""
    for x in sys.argv:  #Runs through all arguments
        strings = x.split("=")  #splits at the =
        if "key" in strings[0]:     #Checks if the variable input is key
            key = strings[1]
        if len(key) > 0:
            string = "GET " + key   #Builds the command string to send
            save.append(string)
            key = ""

for x in save:  #Runs through all command strings
    clientsocket.send(x)    #Sends each command to server
    from_server = clientsocket.recv(1024)   #Gets the reponse
    print(from_server)  #Prints the response

clientsocket.close()    #Close the program
