#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <string.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[])
{	
	FILE * file;
	char str[999];
	int sockfd, cli, portno, clilen, n, count=0;
	char buffer[256];
	char temp[256];
	char * tmp;
	struct  sockaddr_in serv_addr, cli_addr;
	if (argc<2)
	{
		fprintf(stderr, "Error, no port info\n");
		exit(0);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		error("Error in creating socket");
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
	{
		error("Error on bind");
		exit(0);
	}
	listen(sockfd,2);

	clilen = sizeof(cli_addr);
	cli = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (cli == -1)
		{
			error("Error in accepting client");
			exit(0);
		}
	printf("New Client connected from port number %d and IP %s\n",ntohs(cli_addr.sin_port), inet_ntoa(cli_addr.sin_addr));
	n = 1;	
	while(1)
	{
		bzero(buffer,256);
		bzero(temp,256);
		count = 0;
		n = read(cli, buffer, 255);
		printf("Received command: %s\n", buffer);
		strcpy(temp, buffer);
		tmp = strtok (temp," ");
		if(tmp==NULL)
		{
			end(cli, "IMPROPER COMMAND", 16, 0);
			continue;	
		}
		if(strcmp("GET",tmp)==0)
		{	
			tmp = strtok(NULL, "\n");
			printf("%s",tmp );
			if(tmp==NULL)
			{	
				bzero(buffer,256);
				strcpy(buffer,"No Filename Entered");
				n = send(cli, buffer, strlen(buffer),0);
				continue;
			}
			printf("\nTrying to open FILE: %s\n", tmp);
			file = fopen(tmp, "r");
			if (file) 
			{
			  	while (fscanf(file, "%s", str)!=EOF)
			    printf("%s",str);
			  	printf("\n");
			  	fclose(file);
			  	send(cli, "FILE FOUND", 10, 0);
			  	continue;
			}
			printf("Invalid Filename\n");
			send(cli, "INVALID FILENAME", 16, 0);
			continue;
		}
		else
		{
			if(strcmp("BOUNCE",tmp)==0)
			{
				tmp = strtok(NULL, "\n");
				while(tmp!=NULL)
				{	
					printf("%s ",tmp);
					tmp = strtok(NULL, " ");

				}
				printf("\n");
				send(cli, "MESSAGE DISPLAYED", 17, 0);
				continue;
			}
			else
			{
				if (strcmp("EXIT\n",tmp)==0 || strcmp("EXIT",tmp)==0)
				{	
					printf("EXITING  ");
					send(cli, "Client Disconnecting", 21, 0);
					tmp = strtok(NULL, "\n");
					while(tmp!=NULL)
					{	
						printf("%s ",tmp);
						tmp = strtok(NULL, " ");
					}
					printf("\nClient Disconnected\n");
					return 0;
				}
				else
				{
					printf("Not proper command\n");
					send(cli, "IMPROPER COMMAND", 16, 0);
					close(cli);
					close(sockfd);
					continue;
				}
			}
		}
	}
	close(sockfd);
	close(cli);
	printf("Client Disconnected\n");
	return 0;
}