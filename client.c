#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <sys/types.h> 
#include <string.h> 
#include <netdb.h>
#include<arpa/inet.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[255];
	if (argc < 3)
	{
		printf("usage %s\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("Cannot Open Socket");
	}

	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		printf("No Host by this IP\n");
	}

	bzero((char *) &serv_addr , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		error("Couldn't connect");
	}

	int i = 1;
	int a = 1;
	while(i)
	{
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));

		if (n<0)
		{
			error("writing Error");
		}

		a = strcmp("end", buffer);
		if(a==0)
		{
			goto Q;
		}

		bzero(buffer, 255);
		n = read(sockfd, buffer, 255);
		if (n<0)
		{
			error("reading Error");
		}
		printf("Server : %s\n", buffer);

		a = strcmp("end", buffer);
		if(a==0)
		{
			goto Q;
		}
	}

	Q : close(sockfd);
	return 0;

}	
