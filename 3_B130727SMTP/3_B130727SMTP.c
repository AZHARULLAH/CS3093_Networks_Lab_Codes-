
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 200
#define MAXPENDING 5


void closeall(char *errorMessage);
void sendmessages(int clntSocket, int acceptfd);
int createsocket(unsigned short port);
int acceptconn(int servSock);


int main(int argc, char *argv[])
{
    int *servSock;
    int maxDescriptor;
    fd_set sockSet;
    long timeout;
    struct timeval selTimeout;
    int running = 1;
    int noPorts;
    int port;
    unsigned short portNo;

    if (argc < 3)
    {
        fprintf(stderr, "Usage:  %s ...\n", argv[0]);
        exit(1);
    }

    timeout = atol(argv[1]);       
    noPorts = argc - 2;        

    servSock = (int *) malloc(noPorts * sizeof(int));
    maxDescriptor = -1;
 
    for (port = 0; port < noPorts; port++)
    {
        portNo = atoi(argv[port + 2]);
        //printf("######%d",portNo);

        servSock[port] = createsocket(portNo);

        if (servSock[port] > maxDescriptor)
            maxDescriptor = servSock[port];
    }

    printf("Starting server...\n");
    
    
    /////////////mymods

	    int sockfd, connectfd, bindfd, listenfd, acceptfd, clientaddresssize, msgsizenew, i=0, j=0, n, z, temp, arr[10000];
	    struct sockaddr_in myaddress, clientaddress;
	    char msg[2000];
	    sockfd = socket(PF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0)
	    { 
	        printf("Error creating socket\n");
	    }
	    myaddress.sin_family = AF_INET;
	    myaddress.sin_port = htons(3601);
	    myaddress.sin_addr.s_addr = INADDR_ANY;
	    memset(&(myaddress.sin_zero),'\0',8);
	    bindfd = bind(sockfd, (struct sockaddr *)&myaddress, sizeof(myaddress));
	    if (bindfd < 0)
	    { 
	        printf("Error binding socket\n");
	    }
	    printf("Bind conn %d\n", bindfd);
	    //connectfd = connect(sockfd, (struct sockaddr *)&myaddress, sizeof(struct sockaddr));
	    listenfd = listen(sockfd, 10);
	    if (listenfd < 0)
	    { 
	        printf("Error listening to client\n");
	    }
	    printf("Listened to conn %d\n", listenfd);
	    clientaddresssize = sizeof(clientaddress);
	    acceptfd = accept(sockfd, (struct sockaddr *)&clientaddress, &clientaddresssize);
	    if (acceptfd < 0)
	    { 
	        printf("Error accepting conn\n");
	    }
	    printf("Accepted connn %d\n", acceptfd);

    ///////////my mods end
    
    
    while (running)
    {
        FD_ZERO(&sockSet);
        FD_SET(STDIN_FILENO, &sockSet);
        for (port = 0; port < noPorts; port++)
            FD_SET(servSock[port], &sockSet);

        selTimeout.tv_sec = timeout;
        selTimeout.tv_usec = 0;

        if (select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
            {
                //printf("No echo requests for %ld secs...Server still alive\n", timeout);
            }
        else
        {
            if (FD_ISSET(0, &sockSet)) 
            {
                printf("Shutting down server\n");
                getchar();
                running = 0;
            }

            for (port = 0; port < noPorts; port++)
                if (FD_ISSET(servSock[port], &sockSet))
                {
                    printf("Request on port %d:  ", port);
                    sendmessages(acceptconn(servSock[port]), acceptfd);
                }
        }
    }

    for (port = 0; port < noPorts; port++)
        close(servSock[port]);

    free(servSock);

    exit(0);
}

int acceptconn(int servSock)
{
    int clntSock;                    
    struct sockaddr_in echoClntAddr; 
    unsigned int clntLen;            

    clntLen = sizeof(echoClntAddr);
   
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
           &clntLen)) < 0)
        closeall("accept() failed");
   
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    return clntSock;
}

int createsocket(unsigned short port)
{
    int sock;                        
    struct sockaddr_in echoServAddr;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        closeall("socket() failed");
     
    memset(&echoServAddr, 0, sizeof(echoServAddr));   
    echoServAddr.sin_family = AF_INET;                
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    echoServAddr.sin_port = htons(port);              

    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        closeall("bind() failed");

    if (listen(sock, MAXPENDING) < 0)
        closeall("listen() failed");

    return sock;
}
void sendmessages(int clntSocket, int acceptfd)
{
    char echoBuffer[RCVBUFSIZE];       
    int recvMsgSize, pp,z;                   
	char msgsize[2000];
	char messagetobesent[200];
	char receiveremail[200];
	char senderemail[200];

    if ((recvMsgSize = recv(clntSocket, echoBuffer, 200, 0)) < 0)
        closeall("recv() failed");
	else
	{
		strcpy(&senderemail[0],&echoBuffer[0]);
		printf("\nReceived from -  %s\n", &echoBuffer[0]);
	}
    
    if ((recvMsgSize = recv(clntSocket, echoBuffer, 200, 0)) < 0)
        closeall("recv() failed");
	else
	{
		strcpy(&receiveremail[0],&echoBuffer[0]);
		printf("\nReceived email -  %s\n", &echoBuffer[0]);
	}
	
	if ((recvMsgSize = recv(clntSocket, echoBuffer, 200, 0)) < 0)
        closeall("recv() failed");
	else
	{
		strcpy(&messagetobesent[0],&echoBuffer[0]);
		printf("\nReceived message - %s\n", &echoBuffer[0]);
	}
	
    if(send(acceptfd, &senderemail[0], 200, 0) < 0)
    {
        printf("Sending failed\n");
        //return 1;
    }
    
    if(send(acceptfd, &receiveremail[0], 200, 0) < 0)
    {
        printf("Sending failed\n");
        //return 1;
    }
    
    if(send(acceptfd, &messagetobesent[0], 200, 0) < 0)
    {
        printf("Sending failed\n");
        //return 1;
    }

    close(clntSocket);  
}

void closeall(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
