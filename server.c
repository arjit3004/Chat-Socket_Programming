#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <sys/types.h> 
#include <string.h> 


void error(const char *msg)
{
	perror(msg);
	exit(1);

}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("%s\n","Provide Port");
		exit(1);
	}

	int sockfd, newsockfd, portno, n;
	char buffer[255];

	// create socket
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Cannot Open Socket");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// bind socket
	if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		error("Could Not Bind");
	}

	// listen socket
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	int i = 1;
	int a;
	while(i)
	{
		bzero(buffer, 255);
		
		// read
		n = read(newsockfd, buffer, 255);
		if(n < 0)
		{
			error("Cannot read");
		}
		printf("Client : %s\n", buffer);

		a = strcmp("end", buffer);
		if(a==0)
		{
			goto Q;
		}


		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		
		// write
		n = write(newsockfd, buffer, strlen(buffer));
		if(n < 0)
		{
			error("Cannot Write");
		}
		

		a = strcmp("end", buffer);
		if(a==0)
		{
			goto Q;
		}
	}	
	
	// close
	Q : close(newsockfd);
	close(sockfd);
	return 0;
}
