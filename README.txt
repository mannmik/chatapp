chatserve.py

	- to compile and run type in the command line the following, python chatserve.py [PORTNUMBER]
		- if that port is busy you will get an error binding message. Try to compile again with a new port number.
		- example: python chatserve.py 30020
		- chatserve needs to be running on a valid port before starting chatclient
	- once a connection is made with a client you will be able to send and receive messages
	- you can quit from the server side by sending \quit
	-if the user on the client side sends \quit, chatserve will continue listening on the port specified
	-can kill the server with ctrl c

chatclient.c
	- to compile type the following in to the command line, gcc chatclient.c -o client
	- run by entering the following into the command line, ./client [HOSTNAME] [PORTNUMBER]
		- HOSTNAME is the server name that chatserve was started on
		-PORTNUMBER is the port number chatserve was started on
		- example: ./client flip1.engr.oregonstate.edu 30020
	- once connection is made you will be prompted to add your "handle". after this you will be prompted with, handle>, enter your message to send to           the server. Type \quit to end the chat from the client side. 
