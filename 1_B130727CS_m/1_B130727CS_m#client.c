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
	int sockfd, connectfd, z, n, i=0, arr[100000], msgsize, choice;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{ 
        printf("Error creating socket\n");
	}
	char msg[2000], servermsg[2000];
	struct sockaddr_in myaddress;
	myaddress.sin_family = AF_INET;
	myaddress.sin_port = htons(3510);
	myaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(myaddress.sin_zero),'\0',8);
	connectfd = connect(sockfd, (struct sockaddr *)&myaddress, sizeof(struct sockaddr));
	if (connectfd < 0)
	{ 
        printf("Error connecting to server\n");
	}
	printf("Connected to server %d\n", connectfd);
	// while(1)
	// {
		// printf("Enter Msg : \n");
		// scanf("%s",msg);
		printf("Enter N : \n");
		scanf("%d",&n);
		if(n<=0)
		{
			printf("Entered n is invalid\n");
			return 1;
		}
		if(send(sockfd, &n, 4, 0) < 0)
		{
			printf("Sending failed\n");
			return 1;
		}
		printf("Sent n - %d\n", n);
		printf("Enter the nos : \n");
		for(i=0;i<n;i++)
		{
			scanf("%d",&z);
			printf("Sending %d\n", z);
			if(send(sockfd, &z, 4, 0) < 0)
			{
				printf("Sending failed\n");
				return 1;
			}
			printf("Sent %d\n", z);

		}
		printf("Enter your choice\n");
		printf("1) Ascending order\n");
		printf("2) Descending order\n");
		printf("3) exit\n");
		scanf("%d",&choice);
		if(send(sockfd, &choice, 4, 0) < 0)
		{
			printf("Sending failed\n");
			return 1;
		}
		printf("Sent choice - %d\n",choice);
		printf("Receiving sorted nos...\n");
		for (i = 0; i < n; i++)
	    {
    		msgsize = recv(sockfd, &z, 4, 0);
			if(z == -12345)
			{
				break;
				close(sockfd);
			}
    		if (msgsize < 0)
			{
				 
		        printf("Error receiving from socket\n");
			}
	        arr[i] = z;
    	}
		for (i = 0; i < n; i++)
		{
			if(arr[i] == 0)
			{
				break;
				close(sockfd);
			}
			printf("%d\n",arr[i]);
		}
	close(sockfd);
	return 0;
}
