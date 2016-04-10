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

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char const *argv[])
{
	int sockfd, portno, n;
	char buffer[256];
	struct  sockaddr_in serv_addr;
	struct hostent *server;
	if (argc < 3)
	{
		fprintf(stderr, "No proper arguments\n");
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		error("Error in creating socket");
	}

	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr, "ERROR no such host");
		exit(0);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	{
		error("Error on connect");
	}
	while(1){
		printf("Enter command:\n");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		n = send(sockfd, buffer, strlen(buffer),0);
		bzero(buffer, 256);
		n = read(sockfd, buffer, 255);
		if(n < 0)
		{
			error("No command entered\n");
		}
		else
		{	
			printf("%s\n", buffer);
			if(strstr(buffer,"Client Disconnecting")!=0)
			{
				close(sockfd);
				return 0;
			}
		}
	}

	return 0;
}