#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#define MAX_LENGTH 1024

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
  int socket_fd, bytes_read;
  unsigned int address_length;
  char recieve_data[MAX_LENGTH],send_data[MAX_LENGTH];
  struct sockaddr_in server_address , client_address;
  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
  {
    error("socket()");
  }
  int flags = fcntl(socket_fd, F_GETFL);
  flags |= O_NONBLOCK;
  fcntl(socket_fd, F_SETFL, flags);
  //fcntl(socket_fd, F_SETFL, O_NONBLOCK); //set socket to non-blocking
  
  FD_ZERO(&original_socket);
  FD_ZERO(&original_stdin);
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  
  FD_SET(socket_fd, &original_socket);
  FD_SET(socket_fd, &readfds);
  FD_SET(0,&original_stdin);
  FD_SET(0, &writefds);
  
  numfd = socket_fd + 1;
 
  tv.tv_sec = 10;
  tv.tv_usec = 500000;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(5000);
  server_address.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server_address.sin_zero),8);
  if (bind(socket_fd,(struct sockaddr *)&server_address, sizeof(struct sockaddr)) == -1)
  {
    error("bind()");
  }
  address_length = sizeof(struct sockaddr);
  
  //mods
  
  	int choice = 1;
  	
  	//receiving choice from client
  	/*if (FD_ISSET(socket_fd, &readfds))
        { 
		FD_CLR(socket_fd, &readfds);
  		recvfrom(socket_fd, &choice, 4, 0, (struct sockaddr *)&client_address, &address_length);	
  		printf("\nselected choice is %d\n",choice);*/
  	
  	if(choice ==1)
  	{
  	
		  printf("\nUDP_Server Waiting for client to respond...\n");
		  fflush(stdout);
		  printf("Type (q or Q) at anytime to quit\n");

			//int counter = 1;

		  while (1)
		  {
		  	static int counter = 1;
		  
		    readfds = original_socket;
		    writefds = original_stdin;
		    int recieve = select(numfd, &readfds, &writefds, NULL, &tv);
		    
		    	struct pkt 
			{
				int seqnum;
				int acknum;
				char payload[20];
			}packet;
		    
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
		      if (FD_ISSET(socket_fd, &readfds))
		      { 
			FD_CLR(socket_fd, &readfds);
			bytes_read = recvfrom(socket_fd,&packet, sizeof(struct pkt),0,(struct sockaddr *)&client_address, &address_length);
			//recieve_data[bytes_read] = '\0';
			/*if((strcmp(recieve_data , "q") == 0) || (strcmp(recieve_data , "Q") == 0)) 
			{ 
			  printf("\nClient has exited.\n");
			  break;
			}
			printf("\n(%s , %d) said: %s\n",inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port),recieve_data);*/
		
			printf("\nClient sent : \n");
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
		
			sendto(socket_fd, &packet.acknum,4,0,(struct sockaddr *)&client_address, sizeof(struct sockaddr));
			printf("\nSending back ack number - %d\n", packet.acknum);
		
		      }
		      /*else if (FD_ISSET(0, &writefds))
		      {
			FD_CLR(0, &writefds);
			printf("SERVER: ");
			fgets (send_data, MAX_LENGTH, stdin);
			if ((strlen(send_data)>0) && (send_data[strlen (send_data) - 1] == '\n'))
			{
			  send_data[strlen (send_data) - 1] = '\0';
			}
			if ((strcmp(send_data , "q") == 0) || (strcmp(send_data , "Q") == 0)) 
			{ 
			  sendto(socket_fd,send_data,strlen(send_data),0,(struct sockaddr *)&client_address, sizeof(struct sockaddr));
			  break;
			}
			sendto(socket_fd,send_data,strlen(send_data),0,(struct sockaddr *)&client_address, sizeof(struct sockaddr));
			fflush(stdout);
		      }
		      else printf("\nerr occ");*/
		    //}
		    
		    	counter++;
		    
		  }
  	
  	}
  	
  	if(choice == 2)
  	{
  	
  		  printf("\nUDP_Server Waiting for client to respond...\n");
		  fflush(stdout);
		  printf("Type (q or Q) at anytime to quit\n");

			int counter = 1;

		  while (1)
		  {
		  	//static int counter = 1;
		  
		    readfds = original_socket;
		    writefds = original_stdin;
		    int recieve = select(numfd, &readfds, &writefds, NULL, &tv);
		    
		    	struct pkt 
			{
				int seqnum;
				int acknum;
				char payload[20];
			}packet;
		    
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
		      if (FD_ISSET(socket_fd, &readfds))
		      { 
			FD_CLR(socket_fd, &readfds);
			bytes_read = recvfrom(socket_fd,&packet, sizeof(struct pkt),0,(struct sockaddr *)&client_address, &address_length);
			//recieve_data[bytes_read] = '\0';
			/*if((strcmp(recieve_data , "q") == 0) || (strcmp(recieve_data , "Q") == 0)) 
			{ 
			  printf("\nClient has exited.\n");
			  break;
			}
			printf("\n(%s , %d) said: %s\n",inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port),recieve_data);*/
		
			printf("\nClient sent : \n");
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
		
			sendto(socket_fd, &packet.acknum,4,0,(struct sockaddr *)&client_address, sizeof(struct sockaddr));
			printf("\nSending back ack number - %d\n", packet.acknum);
			
			//pkt to client 
			
			char send_data[20];
			
			FD_CLR(0, &writefds);
			printf("\n\nSERVER: ");
			scanf("%s",&send_data[0]);
			//fgets (send_data, MAX_LENGTH, stdin);
			
				//int i;
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
			  sendto(socket_fd, &packet, sizeof(struct pkt), 0, (struct sockaddr *)&client_address, sizeof(struct sockaddr));
			  break;
			}
			else 
			{
			  sendto(socket_fd, &packet, sizeof(struct pkt), 0, (struct sockaddr *)&client_address, sizeof(struct sockaddr));
			 
			  printf("\nSent to client: \n");
			  printf("seqnum - %d, acknum - %d, message - %s \n\n",packet.seqnum, packet.acknum, &send_data[0]);
			  
			  int result;
			  recvfrom(socket_fd,&result, 4,0,(struct sockaddr *)&server_address,&address_length);
			  printf("\nClient sent acknowledgement - %d\n", result+1);
			
		
		      }
		      /*else if (FD_ISSET(0, &writefds))
		      {
			FD_CLR(0, &writefds);
			printf("SERVER: ");
			fgets (send_data, MAX_LENGTH, stdin);
			if ((strlen(send_data)>0) && (send_data[strlen (send_data) - 1] == '\n'))
			{
			  send_data[strlen (send_data) - 1] = '\0';
			}
			if ((strcmp(send_data , "q") == 0) || (strcmp(send_data , "Q") == 0)) 
			{ 
			  sendto(socket_fd,send_data,strlen(send_data),0,(struct sockaddr *)&client_address, sizeof(struct sockaddr));
			  break;
			}
			sendto(socket_fd,send_data,strlen(send_data),0,(struct sockaddr *)&client_address, sizeof(struct sockaddr));
			fflush(stdout);
		      }
		      else printf("\nerr occ");*/
		    //}
		    
		    	counter++;
		    
		  }
  	
  		}
  	}
  
  //mods end
    //}
  close (socket_fd);
  return 0;
}
