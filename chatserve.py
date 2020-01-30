'''
    Author: Mike Mann

    Description: Implements the server side of our chat application. Will listen on a specified port for an incoming connection.
      when a connection is made the server will hold the connection and continue to send and receive messages until the user on the server side enters
      \quit or it receives a ctrl c input. If the client side user enters \quit, this chatserve will break the connection and CONTINUE listening
      on the specified port until it receives a ctrl c signal.

      To run the program enter, python chatserve.py [PORTNUMBER]
      The server name is the name of the server the chatserve.py is being run on. 

'''
import sys
import socket
import string

portNum = int(sys.argv[1])                                                                 #port number
host = socket.gethostname()                                                                #server Name i.e. flip2.engr.oregonstate.edu
                                                                            

'''
    Function: setUpSocket
    Description: sets up our socket and binds it to the specified port from the commmand line
    Parameters: no parameters are needed because we have global variables
    Returns: returns our newly created socket ie file descriptor named socketFD
'''
def setUpSocket():
    socketFD = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        socketFD.bind((host, portNum))                                                     #trys to bind on the host and port
    except:                                                                                #if we couldnt bind then we exit
        print "error binding."
        exit()
    
    return socketFD

'''
    Function: receiveMsg
    Description: Receives a msg from the client
    Parameter: our connection that we accepted
    Return: returns the message that was sent by the client to the server
'''
def receiveMsg(connectionSocket):
    incomingMsg = connectionSocket.recv(512)
    return incomingMsg

'''
    Function: sendMsg
    Description: sendMsg sends gathers and sends the message from ther server to the client
    Parameter: takes our connection that we accepted
    Return: returns the message to sent from the server to the client
'''
def sendMsg(connectionSocket):
    outgoingMsg = raw_input("Server> ")[:500]                                          #gathers a server response

    if "\quit" in outgoingMsg:                                                         #if the user quits then send the \quit message and return
        connectionSocket.send("\quit")
        return outgoingMsg                                                             #return the string gathered from the user
            
    connectionSocket.send("Server> " + outgoingMsg)                                    #sends our server response by combining the server> handle and the message typed in the terminal
    return outgoingMsg                                                                                



'''
    Function: chatHandler
    Description: handles the flow of our chat and listening on a single port
    Parameter: our socket file descriptor that we created
    Return: No return value
'''
def chatHandler(socketFD):
    socketFD.listen(1)                                                                         #will wait and listen. Can only listen to one client at a time
    print "Listening on port " + str(portNum)                                                  #converts port num to a string to show what port the server is listening on 

    continueChat =True                                                                         #Runs our outer loop to see if we should keep listening on a port

    while continueChat:
        connectionSocket, addr = socketFD.accept()                                             #accepts our connection
        while 1:
            incomingMsg = receiveMsg(connectionSocket)                                         #receives our incoming message including username/handl + > + " "
            if "\quit" in incomingMsg:                                                         #check to see if our client quit then break out of inner
                print "Connection Closed."                                                     #loop and listen on port again. Notify connection was closed
                print ""                                                                       #print empty line for clarity
                print "Listening on port " + str(portNum)
                break                                                                          #break out of inner loop
                
            print ""                                                                           #print empty line for clarity
            print incomingMsg                                                                  #displays our message from the client
            print ""                                                                           #print empty line for clarity

            outgoingMsg = sendMsg(connectionSocket)                                            #gathers a server response message

            if "\quit" in outgoingMsg:                                                         #if the user quits then break out. ctrl c also ends the server runnin
                continueChat = False                                                           #\quit was found on server side so also exit outer loop
                break                                                                          #break out of inner loop       
                                                                                    
    socketFD.close()                                                                           #closes our socket


def main():
    chatHandler(setUpSocket())                                                                 #sets up our socket then handles the entire chat


main()                                                                                        
