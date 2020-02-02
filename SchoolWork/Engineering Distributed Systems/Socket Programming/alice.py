#
#   Created by: Markus T Peterson
#   Date: 1/27/2020
#   Assignment 0
#   E510 - ENGINEERING DISTRIBUTED SYSTMS
#

import socket
import sys

save = [] #Used to save all input key value pairs in string form

#Next two lines for connecting to the server socket
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('0.0.0.0', 8080))


if len(sys.argv) > 1: # Used for command line arguments
    key = ""
    value = ""
    for x in sys.argv:  #Runs through all arguments
        strings = x.split("=")  #splits at the =
        if "key" in strings[0]:
            key = strings[1]
        elif "value" in strings[0]:
            value = strings[1]
        if len(key) > 0 and len(value) > 0:
            string = "STORE " + key + "=" + value #Builds the STORE COMMAND
            save.append(string)                   #for the server
            key = ""
            value = ""

for x in save: #Runs through the list of info to send to the server
    clientsocket.send(x)    #Sends the string
    from_server = clientsocket.recv(1024)   #Gets the response
    print(from_server)  #prints the response

clientsocket.close()    #Close connection
