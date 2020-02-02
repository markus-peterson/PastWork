#
#   Created by: Markus T Peterson
#   Date: 1/27/2020
#   Assignment 0
#   E510 - ENGINEERING DISTRIBUTED SYSTMS
#

import os
import sys
#import server

#server.start()
os.system('python server.py &') #starts the server

saveCommand = 'python alice.py' #stores the default command
retvCommand = 'python bob.py'   #stores the default command

if len(sys.argv) > 1: #Checks for arguments with command call
    for x in sys.argv:  #for each argument
        string = x.split("=") #split the command from the data
        if "SAVE" in string[0]: #if to save the data
            saveStringS = string[1].split(",")  #seperate pairs
            for pair in saveStringS:    #for each pair
                kvSplit = pair.split(":")   #seperate the pair
                #and build the start command with the correct arguments
                saveCommand += ' --key=' + kvSplit[0] + ' --value=' + kvSplit[1]
        elif "RETV" in string[0]:   #if data to be retreived
            saveStringR = string[1].split(",")  #split up keys
            for key in saveStringR: #for each key
                retvCommand += ' --key=' + key #add it to the bob start command

os.system(saveCommand)  #call alice.py with args
os.system(retvCommand)  #call bob.py with args in place
