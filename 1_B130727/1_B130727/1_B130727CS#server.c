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
	int sockfd, connectfd, bindfd, listenfd, acceptfd, clientaddresssize, msgsize, i=0, j=0, n, z, temp, arr[10000];
	struct sockaddr_in myaddress, clientaddress;
	char msg[2000];
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{ 
        printf("Error creating socket\n");
	}
	myaddress.sin_family = AF_INET;
	myaddress.sin_port = htons(3501);
	// inet_aton(myip,&myaddr.sin_addr);
	myaddress.sin_addr.s_addr = INADDR_ANY;
	memset(&(myaddress.sin_zero),'\0',8);
	bindfd = bind(sockfd, (struct sockaddr *)&myaddress, sizeof(myaddress));
	if (bindfd < 0)
	{ 
        printf("Error binding socket\n");
	}
	printf("Bind conn %d\n", bindfd);
	//connectfd = connect(sockfd, (struct sockaddr *)&myaddress, sizeof(struct sockaddr));
	listenfd = listen(sockfd, backlog);
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
	msgsize = recv(acceptfd, &n, 4, 0);
	printf("%d***\n", msgsize);
	printf("Received n - %d\n", n);
	for (i = 0; i < n; i++)
    {
    	msgsize = recv(acceptfd, &z, 4, 0);
    	if (msgsize < 0)
		{ 
	        printf("Error receiving from socket\n");
		}
        arr[i] = z;
    }
    printf("Array is :\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\n", arr[i]);
    }
	for (i = 0; i < n; i++)
    {
        for (j = 0; j < (n - i - 1); j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("Sorted array is...\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\n", arr[i]);
    }
    printf("Sending back nos sorted : \n");
    for(i=0;i<n;i++)
	{
		z = arr[i];
		printf("Sending %d\n", z);
		if(send(acceptfd, &z, 4, 0) < 0)
		{
			printf("Sending failed\n");
			return 1;
		}
		printf("Sent %d\n", z);

	}
	//close();
	return 0;
}