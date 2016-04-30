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
	struct sockaddr_in localaddress, clientaddress, rootaddress, authaddress, tldaddress;
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

	rootaddress.sin_family = AF_INET;
	rootaddress.sin_port = htons(rootport);
	rootaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(rootaddress.sin_zero),'\0',8);

	tldaddress.sin_family = AF_INET;
	tldaddress.sin_port = htons(tldport);
	tldaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(tldaddress.sin_zero),'\0',8);

	authaddress.sin_family = AF_INET;
	authaddress.sin_port = htons(authport);
	authaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(authaddress.sin_zero),'\0',8);


	bindfd = bind(sockfd, (struct sockaddr *)&localaddress, sizeof(localaddress));
	if(bindfd < 0)
	{
		printf("Error binding\n");
		exit(1);
	}

	while(1)
	{
		messages = 0;
		printf("********************************************\n");
		char hostname[50];
		recvfrom(sockfd, &hostname, 50, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
		messages++;
		printf("Received hostname from client - %s\n", hostname);
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
		printf("firstpart - %s\n", firstpart);
		printf("secondpart - %s\n", secondpart);
		printf("thirdpart - %s\n", lastpart);

		FILE *fp;
		fp = fopen("cache","r+");
		if(fp == NULL)
		{
			printf("File open failed\n");
			exit(1);
		}
		fseek(fp,0,SEEK_SET);
		char hostbuffer[50];
		char ipbuffer[50];
		int read = fscanf(fp,"%s",&hostbuffer[0]);
		int ird = fscanf(fp,"%s",&ipbuffer[0]);
		fseek(fp,0,SEEK_SET);

		while(read >= 0)
		{
			printf("second part - %s, hostbuffer - %s\n",secondpart,hostbuffer);
			if(strcmp(&secondpart[0],&hostbuffer[0]) == 0)
			{
				printf("Found %s in cache, sending ip to tld\n", hostbuffer);
				char msg[50] = "Present in cache";
				printf("%s\n", msg);
				sendto(sockfd, &msg, 50, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
				messages++;
				
				char tldreply[50];
				sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
				messages++;
				sendto(sockfd, &ipbuffer, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
				messages++;
				recvfrom(sockfd, &tldreply, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
				messages++;
				printf("Received %s as reply from tld\n", tldreply);
				
				char authreply[50];
				sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
				messages++;
				sendto(sockfd, &tldreply, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
				messages++;
				recvfrom(sockfd, &authreply, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
				messages++;
				printf("Received %s as reply from auth\n", authreply);
				
				sendto(sockfd, &authreply, 50, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
				messages++;
				
				break;
			}
			read = fscanf(fp,"%s",&hostbuffer[0]);
        		ird = fscanf(fp,"%s",&ipbuffer[0]);
		}
		if(read < 0)
		{
			//printf("here\n");
			char msg[50] = "Not in cache";
			printf("%s\n", msg);
			sendto(sockfd, &msg, 50, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
			messages++;
			printf("Not in cache.....Querying to root\n");
			//fclose(fp);
	
			//sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
			char rootreply[50];
			sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&rootaddress, sizeof(rootaddress));
			messages++;
			recvfrom(sockfd, &rootreply, 50, 0, (struct sockaddr *)&rootaddress, sizeof(rootaddress));
			messages++;
			printf("Received %s as reply from root\n", rootreply);

			if(strcmp(&rootreply[0],"Hostname not found") == 0)
			{
				printf("NO HOSTNAME FOUND\n");
				exit(1);
			}
			
			if(strcmp(&rootreply[0],"Hostname not found") != 0)
			{
				int temp1 = fprintf(fp,"%s",&secondpart[0]);
				int temp2 = fprintf(fp," ");
				int temp3 = fprintf(fp,"%s",&rootreply[0]);
				int temp4 = fprintf(fp,"\n");
				printf("Writing %s to cache \n", secondpart);
				printf("Writing %s to cache \n", rootreply);
			}

			char tldreply[50];
			sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
			messages++;
			sendto(sockfd, &rootreply, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
			messages++;
			recvfrom(sockfd, &tldreply, 50, 0, (struct sockaddr *)&tldaddress, sizeof(tldaddress));
			messages++;
			printf("Received %s as reply from tld\n", tldreply);

			if(strcmp(&tldreply[0],"Hostname not found") == 0)
			{
				printf("NO HOSTNAME FOUND\n");
				exit(1);
			}

			char authreply[50];
			sendto(sockfd, &hostname, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
			messages++;
			sendto(sockfd, &tldreply, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
			messages++;
			recvfrom(sockfd, &authreply, 50, 0, (struct sockaddr *)&authaddress, sizeof(authaddress));
			messages++;
			printf("Received %s as reply from auth\n", authreply);

			if(strcmp(&authreply[0],"Hostname not found") == 0)
			{
				printf("NO HOSTNAME FOUND\n");
				exit(1);
			}

			sendto(sockfd, &authreply, 50, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
			messages++;
		}

		fclose(fp);
		
		sendto(sockfd, &messages, 4, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
		
		printf("********************************************\n");	

	}	

	close(sockfd);
	return 0;
}
