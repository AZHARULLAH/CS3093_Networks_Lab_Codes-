#include <stdio.h>  
#include <string.h>
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 

#define myip "127.0.0.1"
#define backlog 10

int main()
{
	int sockfd, connectfd, z, n, i=0, arr[100000], msgsize;
	char message[200];
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{ 
        printf("Error creating socket\n");
	}
	char msg[2000], servermsg[2000];
	struct sockaddr_in myaddress;
	myaddress.sin_family = AF_INET;
	myaddress.sin_port = htons(3601);
	myaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(myaddress.sin_zero),'\0',8);
	connectfd = connect(sockfd, (struct sockaddr *)&myaddress, sizeof(struct sockaddr));
	if (connectfd < 0)
	{ 
        printf("Error connecting to server\n");
	}
	printf("Connected to server %d\n", connectfd);

	while(1)
	{	
		char sender[200], receiver[200], message[200];

		msgsize = recv(sockfd, &sender[0], 200, 0);
		if (msgsize < 0)
		{ 
		    //printf("Error receiving from socket\n");
		}
		else
		{
			printf("\n\nHi!! You have an email : \n");
			printf("\nSender : %s\n", &sender[0]);
		}
		
		msgsize = recv(sockfd, &receiver[0], 200, 0);
		if (msgsize < 0)
		{ 
		    //printf("Error receiving from socket\n");
		}
		else
		{
			//printf("Your message is %s\n", &receiver[0]);
		}
		
		msgsize = recv(sockfd, &message[0], 200, 0);
		if (msgsize < 0)
		{ 
		    //printf("Error receiving from socket\n");
		}
		else
		{
			printf("\nMessage : %s\n", &message[0]);
			printf("\n\n***************\n\n");
		}
	}

	close(sockfd);
	return 0;
}
