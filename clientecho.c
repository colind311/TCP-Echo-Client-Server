#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

//TCP client that receives back a message it sends to server

#define MAXBUFFER 1024

#define SERVPORT 20000

int main() {

	setvbuf( stdout, NULL, _IONBF, 0 );

	int sockfd, n;
	struct sockaddr_in servaddr;

	char buff[MAXBUFFER + 1];
	bzero(buff, MAXBUFFER + 1);
	char rcvline[MAXBUFFER + 1];
	bzero(rcvline, MAXBUFFER + 1);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("sockfd failed");
		return EXIT_FAILURE;
	}

	struct hostent * hp = gethostbyname( "127.0.0.1" );

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVPORT);
	memcpy( (void *)&servaddr.sin_addr, (void *)hp->h_addr, hp->h_length );
	if(connect(sockfd, (struct sockaddr * ) &servaddr, sizeof(servaddr)) < 0) {
		perror("connect failed");
		return EXIT_FAILURE;
	}

	while( (fgets(buff, MAXBUFFER, stdin) ) != NULL) {

		write(sockfd, buff, strlen(buff));

		n = read(sockfd, rcvline, MAXBUFFER);

		rcvline[n] = '\0';
		printf("%s", rcvline);
	}

	close(sockfd);

	return EXIT_SUCCESS;
}

