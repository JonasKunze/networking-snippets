/*
 ============================================================================
 Name        : TCPClient
 Author      : Jonas Kunze (kunze.jonas@gmail.com)
 Description : A simple synchronous TCP server reading up to 255 Bytes and
 answering with a small message in native C
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

void error(const char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}

	int flag = 0;
	// Switch off Nagle's algorithm
	setsockopt(sockfd, /* socket affected */
	IPPROTO_TCP, /* set option at TCP level */
	TCP_NODELAY, /* name of option */
	(char *) &flag, /* the cast is historical cruft */
	sizeof(int)); /* length of option value */

	server = gethostbyname("localhost");
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	/*
	 * Connect to the server listening to port 1234 running on localhost
	 */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(1234);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	write(sockfd, "Ping", 4);
	read(sockfd, buffer, sizeof(buffer));
	printf("Response from Server: %s\n", buffer);

	close(sockfd);
	return 0;
}
