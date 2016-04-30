#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define maxbuffersize 32

void reporterror(char *errorMessage);

int main(/*int argc, char *argv[]*/)
{
    int sock;
    struct sockaddr_in echoServAddr;
    //char echoServPort = atoi(argv[1]);
    char echoBuffer[maxbuffersize];
    unsigned int echoStringLen;
    int bytesRcvd, totalBytesRcvd;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        reporterror("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family      = AF_INET;
    echoServAddr.sin_addr.s_addr = INADDR_ANY;
    echoServAddr.sin_port        = htons(3901);

    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        reporterror("connect() failed");
    
    printf("\n Enter your user email ID : ");
    char useremail[200];
    scanf("%s",&useremail[0]);
    printf("\n Enter your password : ");
    char userpassword[200];
    scanf("%s",&userpassword[0]);
    FILE *fp;
	fp = fopen("login.txt","r");
	if(fp == NULL)
	{
		printf("File open failed\n");
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	char fileemail[200];
	char filepassword[200];
	char fileport[200];
	int read = fscanf(fp,"%s",&fileemail[0]);
	int ird = fscanf(fp,"%s",&filepassword[0]);
	int ird2 = fscanf(fp,"%s",&fileport[0]);
	fseek(fp,0,SEEK_SET);

	while(read >= 0)
	{
		//printf("email - %s %s",&fileemail[0],&useremail[0]);
		if(strcmp(&fileemail[0],&useremail[0]) == 0)
		{
			if(strcmp(&filepassword[0],&userpassword[0]) == 0)
			{
				printf("\nLogin successful!! \n Welcome %s",&useremail[0]);
				
				printf("\nEnter receiver email : ");
				char receiveremail[200];
				scanf("%s",&receiveremail[0]);
				printf("\nEnter message : ");
				char messagetobesent[200];
				scanf("%s",&messagetobesent[0]);
				
				printf("\nSending the email to %s\n\n", &receiveremail[0]);
				
				if(send(sock, &useremail[0], 200, 0) < 0)
				{
					printf("Sending failed\n");
				}
				
				if(send(sock, &receiveremail[0], 200, 0) < 0)
				{
					printf("Sending failed\n");
				}
				
				if(send(sock, &messagetobesent[0], 200, 0) < 0)
				{
					printf("Sending failed\n");
				}
				
				break;
			}
		}
		read = fscanf(fp,"%s",&fileemail[0]);
    	ird = fscanf(fp,"%s",&filepassword[0]);
    	ird2 = fscanf(fp,"%s",&fileport[0]);
	}
	if(read < 0)
	{
		printf("\nLogin not successful!! You are not registered or email and password are wrong...\n\n");
	}
    

    //close(sock);
    return 0;
}

void reporterror(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
