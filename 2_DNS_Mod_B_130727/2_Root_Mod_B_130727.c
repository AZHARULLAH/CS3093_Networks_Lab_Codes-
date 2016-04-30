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
	int sockfd, bindfd;
	struct sockaddr_in rootaddress, localaddress;
	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		printf("Error creating socket \n");
		exit(1);
	}

	rootaddress.sin_family = AF_INET;
	rootaddress.sin_port = htons(rootport);
	rootaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(rootaddress.sin_zero),'\0',8);

	localaddress.sin_family = AF_INET;
	localaddress.sin_port = htons(localport);
	localaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(localaddress.sin_zero),'\0',8);


	bindfd = bind(sockfd, (struct sockaddr *)&rootaddress, sizeof(rootaddress));
	if(bindfd < 0)
	{
		printf("Error binding\n");
		exit(1);
	}

	FILE *fp;
	fp = fopen("roots","r+");
	if(fp == NULL)
	{
		printf("File open failed\n");
		exit(1);
	}
	while(1)
	{
		printf("********************************************\n");

		fseek(fp,0,SEEK_SET);

		char hostname[50];
		recvfrom(sockfd, &hostname, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
		printf("received hostname from localdns- %s\n", hostname);
		int i, count = 0, dest=strlen(hostname), count1 = 0;
		char lastpart[50] = {0};
		char secondpart[50] = {0};
		char firstpart[50] = {0};
		for(i = strlen(hostname)-1 ; i>=0 ; i--)
		{
			//printf("Checking %c\n", hostname[i]);
			count++;
			if(hostname[i]=='.' || i==0)
			{
				count1++;
				if(count1 > 3)
					break;
				if(count1==1)
				  	strcpy(lastpart,&hostname[i+1]);
				if(count1==2)
				  	strcpy(secondpart,&hostname[i+1]);
				if(count1==3)
				{ if(i==0)
			        strcpy(firstpart,&hostname[i]);
                   else
			        strcpy(firstpart,&hostname[i+1]);
                }  	 
			}
		}
		// printf("firstpart - %s\n", firstpart);
		// printf("secondpart - %s\n", secondpart);
		// printf("thirdpart - %s\n", lastpart);
	
		char hostbuffer[50];
		char ipbuffer[50];
		int read = fscanf(fp,"%s",&hostbuffer[0]);
		int ird = fscanf(fp,"%s",&ipbuffer[0]);
		while(read >= 0)
		{
			if(strcmp(&lastpart[0],&hostbuffer[0]) == 0)
			{
				sendto(sockfd, &ipbuffer, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
				printf("Found %s, sending filename to localdns\n", hostbuffer);
				break;
			}
			read = fscanf(fp,"%s",&hostbuffer[0]);
        	ird = fscanf(fp,"%s",&ipbuffer[0]);
		}
		if(read < 0)
		{
			//printf("here\n");
			char msg[50] = "Hostname not found";
			printf("%s\n", msg);
			sendto(sockfd, &msg, 50, 0, (struct sockaddr *)&localaddress, sizeof(localaddress));
		}

		printf("********************************************\n");
	}	

	close(sockfd);
	return 0;
}