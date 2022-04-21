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

//TCP server that receives a message from client and sends it back

#define LISTENQ 5

#define MAXBUFFER 1024

#define SERVPORT 20000

int main() {

	setvbuf( stdout, NULL, _IONBF, 0 );

	int listenfd, connfd;
	struct sockaddr_in servaddr;

	char buff[MAXBUFFER + 1];
	bzero(buff, MAXBUFFER + 1);
	char rcvline[MAXBUFFER + 1];
	bzero(rcvline, MAXBUFFER + 1);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1) {
		perror("socket failed");
		return EXIT_FAILURE;
	}

	struct hostent * hp = gethostbyname( "127.0.0.1" );
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVPORT);
	memcpy( (void *)&servaddr.sin_addr, (void *)hp->h_addr, hp->h_length );

	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind failed");
		return EXIT_FAILURE;
	}

	if(listen(listenfd, LISTENQ) < 0) {
		perror("listen failed");
		return EXIT_FAILURE;
	}

	printf("SERVER: ready to start accepting\n");

	while(1) {
		connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
		if(connfd < 0) {
			perror("accept failed");
			continue;
		}

	    if( read(connfd, rcvline, sizeof(rcvline)) < 0) {
	    	perror("read failed");
	    	return EXIT_FAILURE;
	    }

	    memcpy(buff, rcvline, sizeof(rcvline));
	    printf("Buffer value: %s", buff);
	    write(connfd, buff, strlen(buff));
	    close(connfd);
	}

	close(listenfd);

	return EXIT_SUCCESS;
}