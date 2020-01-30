/**************************************************
**Author: Mike Mann
**Description: This the client side of our server client chat project. CLient side gathers a user's handle or username no more than 10 characters.
** Gathers a message of up to 500 characters and then sends the username and message to the server and waits for a response from the server.
** This communication continues until the user types \quit or the signal is terminated on the server side.
** To run, compile with gcc chatclient.c -o client, then run with the following format, ./client [ServerHostName] [PORTNUMBER]
** 
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <fcntl.h>

/***************************************************
**Function: initiateConnect
**Parameters: our socket file descriptor, our server host name, and the port number
**Description: establishes our connection with the server
****************************************************/
void initiateConnect(int socketFD, struct hostent* sHostInfo, int portNum)
{
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    memset((char*)&serverAddress, '\0', sizeof(serverAddress));                                   //clears out our server address struct
    serverAddress.sin_family = AF_INET;                                                             
    serverAddress.sin_port = htons(portNum);                                                      //hold our port  number
    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)sHostInfo->h_addr, sHostInfo->h_length); //copy our address

    connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
  
}

/**************************************************
**Function: getName
**Parameters: pointer to char string name.
**Description: Function gathers the user's handle or username and stores it in the char string name.
***************************************************/

void getName(char *n)
{
    printf("Please enter your desired one word name up to 10 characters: ");
    printf("\n");                                                                                   //print newline for clarity
    memset(n, '\0', sizeof(n));                                                                     //empty out our name for the user
    fgets(n, 10, stdin);                                                                            //take up to 10 characters from input
    n[strcspn(n, "\n")] = '\0';                                                                     //replace the newline character added by fgets

}

/**************************************************
**Function: getMsg
**Parameters: pointer to char string buffer, finalMsg, and name/username for character
**Description: This function prompts the user for the message they want to send. It gather the message in the 
** msg string passed in. Once the msg is gathered and stored in msg we check to see if the user has chose to quit.
** if so, we return 0 and exit the function. If the user did not quit, then combine the user handle name passed in through
** the usr parameter, with the msg we gathered and store them in the final location passed in as paramater final. This
** is our final message that we will send over, then we return 1.
***************************************************/
int getMsg(char *msg, char *final, char *usr)
{
    memset(msg, '\0', sizeof(msg));
    memset(final, '\0', sizeof(final));
    printf("\n%s>\n", usr);
    fgets(msg, 500, stdin);                                                                         //gathr up to 500 characters for the mesg to be sent
    msg[strcspn(msg, "\n")] = '\0';                                                                 //remove the newline character from fgets

    if(strstr(msg, "\\quit") != NULL)                                                               //check to see if the user wanted to quit
    {
        sprintf(final, "%s> %s", usr, msg);                                                         //append the user provided name to our mesg
        printf("\nGood-bye!\n");
        return 0;                                                                                   //if the client typed quit then break out of the chat loop
    }   
    else
    {
        sprintf(final, "%s> %s", usr, msg);                                                         //append the user provided name to our mesg
        return 1;
    }
}   

/******************************************
Function: sendMsg
Parameters: our socket file descriptor, and our char string message to be sent
Description: sends a message to the server
*****************************************/
void sendMsg(int socketFD, char* msg)
{
    send(socketFD, msg, strlen(msg), 0);
}

/******************************************
Function: receiveMsg
Parameters: our socket file descriptor, and our char string where the recv message should be stored
Description: receives a msg from the client and stores it in the char string that was passed in.
*****************************************/
void receiveMsg(int socketFD, char* msg)
{
    recv(socketFD, msg, 500, 0);                                                                    //receive message
}

int main(int argc, char *argv[])
{
    int socketFD, portNum, charsSent, charsRec;
    struct hostent* serverHostInfo;
    char buffer[501];                                                                               //501 to enable the \n character if the user enters max 500 during fgets
    char finalMsg[512];                                                                             // ten char name + > + " "; and up to 500 char msg
    char name[11];                                                                                  //user specified name up to size 10 and one char \n after fgets

    if(argc < 3)                                                                                    //make sure we have the proper number of args
    {
        printf("Invalid number of arguments. Run program with ./chatclient HOSTNAME PORTNUMBER\n");
        return;
    }

    portNum = atoi(argv[2]);                                                                        //takes the port number argument and converts it to an integer

    serverHostInfo = gethostbyname(argv[1]);                                                        //get the server host name

    socketFD = socket(AF_INET, SOCK_STREAM, 0);                                                     //create socket for TCP with sock_stream

    getName(name);                                                                                  //gets user input name less than 10 characters
    initiateConnect(socketFD, serverHostInfo, portNum);            

    while(1)                                                                                        //loop controls our chat and continues until client types quit, ctrl c, or sends a msg to a close connection
    {
      
       if(getMsg(buffer, finalMsg, name))                                                           //if we were able to gather and package a msg and the user didnt quit. send and recv from server.                                   
       {                                      
            sendMsg(socketFD, finalMsg);                                                            //send our msg to the server
            memset(buffer, '\0', sizeof(buffer));                                                   //reset our buffer to get ready to receive a mssg
            receiveMsg(socketFD, buffer);                                                           //read and rec our data                                        
            
            if(strstr(buffer, "\\quit") != NULL)                                                    //see of our user on server side quit and closed connection
            {
                printf("\nServer closed connection. Good-Bye.\n");                                  //if they quit, print a closing message and break the loop
                break;
            }
            printf("\n%s\n", buffer); 
        }                                                                                           //display our received data                                         
        else
        {
            sendMsg(socketFD, finalMsg);                                                            //if the user typed \quit send to server and break out of chat
            break;
        }
    }


    close(socketFD);                                                                                //close our socket
    return 0;
}



