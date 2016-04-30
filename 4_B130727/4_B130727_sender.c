#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#define MAX_LENGTH 1024
#define HOST "localhost"

void error(char *message)
{
  perror(message);
  exit(1);
}

int main()
{
  fd_set original_socket;
  fd_set original_stdin;
  fd_set readfds;
  fd_set writefds;
  struct timeval tv;
  int numfd;
  int socket_fd,bytes_recieved;
  unsigned int address_length;
  struct sockaddr_in server_address;
  struct hostent *host;
  char send_data[MAX_LENGTH],recieve_data[MAX_LENGTH];

  host = (struct hostent *) gethostbyname((char *)HOST);

  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    error("socket()");
  }

  int flags = fcntl(socket_fd, F_GETFL);
  flags |= O_NONBLOCK;
  fcntl(socket_fd, F_SETFL, flags);
  //fcntl(socket_fd, F_SETFL, O_NONBLOCK); 
  
  FD_ZERO(&original_socket);
  FD_ZERO(&original_stdin);
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  
  //Stop and wait transmission (without concurrency)
  FD_SET(socket_fd, &original_socket);
  FD_SET(socket_fd, &readfds);
  FD_SET(0,&original_stdin);
  FD_SET(0, &writefds);
  numfd = socket_fd + 1;
  
  tv.tv_sec = 10;
  tv.tv_usec = 500000;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(5000);
  server_address.sin_addr = *((struct in_addr *)host->h_addr);
  bzero(&(server_address.sin_zero),8);
  address_length = sizeof(struct sockaddr);
  
  //mods
  
  	//menu
  	
  	printf("\n\nSelect an option...\n 1) Stop and wait transmission (without concurrency) \n 2) Stop and wait transmission (with concurrency) \n 3) Selective repeat (without concurrency) \n 4) Selective repeat (with concurrency) \n\n ");
  	int choice;
  	scanf("%d",&choice);
  	if(choice == 1)
  	{
  		//telling server that choice is 1
  		sendto(socket_fd, &choice, 4, 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
  		printf("\nsent selected choice as %d to server\n",choice);
  		
  		int counter = 1;
  		
  		struct pkt 
		{
			int seqnum;
			int acknum;
			char payload[20];
		}packet;
  	
		  printf("Type (q or Q) at anytime to quit\n");
		  while (1)
		  {
		    readfds = original_socket;
		    writefds = original_stdin;
		    int recieve = select(numfd, &readfds, &writefds, NULL, &tv);
		    
		    	packet.seqnum = counter;
		    	packet.acknum = -1;
		    
		    /*if (recieve == -1) 
		    {
		      perror("select");
		    } 
		    else if (recieve == 0) 
		    {
		      printf("Timeout occurred!  No data after 10.5 seconds.\n");
		    } 
		    else 
		    {*/
		      /*if (FD_ISSET(socket_fd, &readfds))
		      {
			FD_CLR(socket_fd, &readfds);
			bytes_recieved = recvfrom(socket_fd,recieve_data, sizeof(recieve_data),0,(struct sockaddr *)&server_address,&address_length);
			recieve_data[bytes_recieved]= '\0';
			if((strcmp(recieve_data , "q") == 0) || (strcmp(recieve_data , "Q") == 0))
			{ 
				printf("\nServer has exited\n");
				break;
			}
			printf("\n(%s , %d) said: %s\n",inet_ntoa(server_address.sin_addr),ntohs(server_address.sin_port),recieve_data);
		      }
		      else if (FD_ISSET(0, &writefds))
		      {*/
			FD_CLR(0, &writefds);
			printf("CLIENT: ");
			scanf("%s",&send_data[0]);
			//fgets (send_data, MAX_LENGTH, stdin);
			
				int i;
				for(i=0;i<strlen(send_data);i++)
				{
					packet.payload[i] = send_data[i];
				}
			
			if ((strlen(send_data)>0) && (send_data[strlen (send_data) - 1] == '\n'))
			{ 
			  send_data[strlen (send_data) - 1] = '\0';
			}
			if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0)
			{ 
			  sendto(socket_fd, &packet, sizeof(struct pkt), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
			  break;
			}
			else 
			{
			  sendto(socket_fd, &packet, sizeof(struct pkt), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
			 
			  printf("\nSent to server: \n");
			  printf("seqnum - %d, acknum - %d, message - %s \n\n",packet.seqnum, packet.acknum, &send_data[0]);
			  
			  int result;
			  recvfrom(socket_fd,&result, 4,0,(struct sockaddr *)&server_address,&address_length);
			  printf("\nServer sent acknowledgement - %d\n", result+1);
			}
		      //}
		      //else printf("error occ");
		    //}
		    
		    	counter++;
		  }
  	
  	}
  	
  	if(choice == 2)
  	{
  		//telling server that choice is 2
  		//sendto(socket_fd, &choice, 4, 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
  		printf("\nsent selected choice as %d to server\n",choice);
  		
  		int counter = 1;
  		
  		struct pkt 
		{
			int seqnum;
			int acknum;
			char payload[20];
		}packet;
  	
		  printf("Type (q or Q) at anytime to quit\n");
		  while (1)
		  {
		    readfds = original_socket;
		    writefds = original_stdin;
		    int recieve = select(numfd, &readfds, &writefds, NULL, &tv);
		    
		    	packet.seqnum = counter;
		    	packet.acknum = -1;
		    
		    /*if (recieve == -1) 
		    {
		      perror("select");
		    } 
		    else if (recieve == 0) 
		    {
		      printf("Timeout occurred!  No data after 10.5 seconds.\n");
		    } 
		    else 
		    {*/
		      /*if (FD_ISSET(socket_fd, &readfds))
		      {
			FD_CLR(socket_fd, &readfds);
			bytes_recieved = recvfrom(socket_fd,recieve_data, sizeof(recieve_data),0,(struct sockaddr *)&server_address,&address_length);
			recieve_data[bytes_recieved]= '\0';
			if((strcmp(recieve_data , "q") == 0) || (strcmp(recieve_data , "Q") == 0))
			{ 
				printf("\nServer has exited\n");
				break;
			}
			printf("\n(%s , %d) said: %s\n",inet_ntoa(server_address.sin_addr),ntohs(server_address.sin_port),recieve_data);
		      }
		      else if (FD_ISSET(0, &writefds))
		      {*/
			FD_CLR(0, &writefds);
			printf("CLIENT: ");
			scanf("%s",&send_data[0]);
			//fgets (send_data, MAX_LENGTH, stdin);
			
				int i;
				for(i=0;i<strlen(send_data);i++)
				{
					packet.payload[i] = send_data[i];
				}
			
			if ((strlen(send_data)>0) && (send_data[strlen (send_data) - 1] == '\n'))
			{ 
			  send_data[strlen (send_data) - 1] = '\0';
			}
			if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0)
			{ 
			  sendto(socket_fd, &packet, sizeof(struct pkt), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
			  break;
			}
			else 
			{
			  sendto(socket_fd, &packet, sizeof(struct pkt), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
			 
			  printf("\nSent to server: \n");
			  printf("seqnum - %d, acknum - %d, message - %s \n\n",packet.seqnum, packet.acknum, &send_data[0]);
			  
			  int result;
			  recvfrom(socket_fd,&result, 4,0,(struct sockaddr *)&server_address,&address_length);
			  printf("\nServer sent acknowledgement - %d\n", result+1);
			  
			  //rev pkt from server
			  FD_CLR(0, &writefds);
			  recvfrom(socket_fd, &packet, sizeof(struct pkt), 0, (struct sockaddr *)&server_address, &address_length);
			  printf("\nServer sent : \n");
			  printf("\n seqnum - %d and acknum - %d \n", packet.seqnum, packet.acknum);
		
			  int i=0;
			  for(i=0;i<sizeof(struct pkt) - 8;i++)
			  {
			  	recieve_data[i] = packet.payload[i];
			  }
		
			  printf("\nMessage - %s\n",&recieve_data[0]);
			  //printf("\ngot into 1 %d\n",counter);
		
			  if(packet.acknum == -1)
			  {
				packet.acknum = packet.seqnum;
			  }
		
			  sendto(socket_fd, &packet.acknum,4,0,(struct sockaddr *)&server_address, sizeof(struct sockaddr));
			  printf("\nSending back ack number - %d\n", packet.acknum);
			}
		      //}
		      //else printf("error occ");
		    //}
		    
		    	counter++;
		  }
  	
  	}
  
  //mods end
  
  /*FD_SET(socket_fd, &original_socket);
  FD_SET(socket_fd, &readfds);
  FD_SET(0,&original_stdin);
  FD_SET(0, &writefds);
  numfd = socket_fd + 1;
  
  tv.tv_sec = 10;
  tv.tv_usec = 500000;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(5000);
  server_address.sin_addr = *((struct in_addr *)host->h_addr);
  bzero(&(server_address.sin_zero),8);
  address_length = sizeof(struct sockaddr);
  printf("Type (q or Q) at anytime to quit\n");
  while (1)
  {
    readfds = original_socket;
    writefds = original_stdin;
    int recieve = select(numfd, &readfds, &writefds, NULL, &tv);
    if (recieve == -1) 
    {
      perror("select");
    } 
    else if (recieve == 0) 
    {
      printf("Timeout occurred!  No data after 10.5 seconds.\n");
    } 
    else 
    {
      if (FD_ISSET(socket_fd, &readfds))
      {
        FD_CLR(socket_fd, &readfds);
        bytes_recieved = recvfrom(socket_fd,recieve_data, sizeof(recieve_data),0,(struct sockaddr *)&server_address,&address_length);
        recieve_data[bytes_recieved]= '\0';
        if((strcmp(recieve_data , "q") == 0) || (strcmp(recieve_data , "Q") == 0))
        { 
        printf("\nServer has exited\n");
        break;
        }
        printf("\n(%s , %d) said: %s\n",inet_ntoa(server_address.sin_addr),ntohs(server_address.sin_port),recieve_data);
        }
      else if (FD_ISSET(0, &writefds))
        {
        FD_CLR(0, &writefds);
        printf("CLIENT: ");
        fgets (send_data, MAX_LENGTH, stdin);
        if ((strlen(send_data)>0) && (send_data[strlen (send_data) - 1] == '\n'))
        { 
          send_data[strlen (send_data) - 1] = '\0';
        }
          if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0)
        { 
          sendto(socket_fd, send_data, strlen(send_data), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
          break;
        }
        else 
        {
          sendto(socket_fd, send_data, strlen(send_data), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
        }
      }
      else printf("error occ");
    }
  }*/
  close (socket_fd);
}
