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
#define clientport 4500
#define localport 4501
#define rootport 4502
#define tldport 4503
#define authport 4504

int main()
{
	int sockfd, bindfd, messages = 0;
	struct sockaddr_in clientaddress, localaddress;
	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		printf("Error creating socket \n");
		exit(1);
	}

	clientaddress.sin_family = AF_INET;
	clientaddress.sin_port = htons(clientport);
	clientaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(clientaddress.sin_zero),'\0',8);

	localaddress.sin_family = AF_INET;
	localaddress.sin_port = htons(localport);
	localaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(localaddress.sin_zero),'\0',8);

	// rootaddress.sin_family = AF_INET;
	// rootaddress.sin_port = htons(rootport);
	// rootaddress.sin_addr.s_addr = INADDR_ANY;
	// memset(&(rootaddress.sin_zero),'\0',8);

	// tldaddress.sin_family = AF_INET;
	// tldaddress.sin_port = htons(tldport);
	// tldaddress.sin_addr.s_addr = INADDR_ANY;
	// memset(&(tldaddress.sin_zero),'\0',8);

	// authaddress.sin_family = AF_INET;
	// authaddress.sin_port = htons(authport);
	// authaddress.sin_addr.s_addr = INADDR_ANY;
	// memset(&(authaddress.sin_zero),'\0',8);

	bindfd = bind(sockfd, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
	if(bindfd < 0)
	{
		printf("Error binding\n");
		exit(1);
	}

	char command[50];
	char hostname[50];
	
		while(1)
		{
			printf("********************************************\n");
			printf("Enter nslookup hostname or exit to exit\n");
			scanf("%s",&command[0]);
			scanf("%s",&hostname[0]);
			//printf("%s\n", &hostname[0]);
			if(strcmp(&command[0],"exit") == 0)
			break;
			if(strcmp(&command[0],"nslookup") == 0)
			{
				sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
				recvfrom(sockfd, &hostname, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
				printf("Cache replied - %s\n", hostname);

				if(strcmp(&hostname[0],"Present in cache") == 0)
				{
					recvfrom(sockfd, &hostname, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
					printf("Local dns sent - %s\n", hostname);
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
					printf("IP ADDRESS IS %s\n", hostname);
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				}
				else if(strcmp(&hostname[0],"Not in cache") == 0)
				{
					recvfrom(sockfd, &hostname, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
					printf("Local dns sent - %s\n", hostname);
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
					printf("IP ADDRESS IS %s\n", hostname);
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				}

				// char rootreply[50];
				// sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&rootaddress, sizeof(rootaddress));
				// recvfrom(sockfd, &rootreply, 50, 0, (struct sockaddr *)&rootaddress, sizeof(rootaddress));
				// printf("Received %s as reply from root\n", rootreply);

				// char tldreply[50];
				// sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
				// sendto(sockfd, &rootreply, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
				// recvfrom(sockfd, &tldreply, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
				// printf("Received %s as reply from tld\n", tldreply);

				// char authreply[50];
				// sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
				// sendto(sockfd, &tldreply, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
				// recvfrom(sockfd, &authreply, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
				// printf("Received %s as reply from auth\n", authreply);
			}
			
			recvfrom(sockfd, &messages, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
			printf("Total messages sent : %d\n",messages);

		}
	
	close(sockfd);
	return 0;
}
