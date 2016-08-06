/*
 * main.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: steven
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int main(void) {
	int sockfd;
	int retcode;
	struct sockaddr_in server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("Error opening socket");
		exit(-1);
	}

	memset(&server_addr, 0, sizeof(server_addr)); /* Zero out structure */

	server_addr.sin_family = AF_INET;                 			// Address family to use
	server_addr.sin_addr.s_addr = inet_addr("192.168.210.159"); 	// IP address to use
	server_addr.sin_port = htons(8888);           				// Port num to use

	retcode = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (retcode < 0) {
		perror("ERROR connecting");
		exit(-1);
	}

	signal(SIGPIPE, SIG_IGN);
//	signal(SIGPIPE, PIPE_handler);

	int sendbuff;
	socklen_t optlen = sizeof(sendbuff);
	int res = getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, &optlen);

	if (res == -1)
		printf("Error getsockopt one");
	else
		printf("send buffer size = %d\n", sendbuff);

	printf("Send Loop\n");
	char buf[1024];
	for (int i = 0; i < 100; i++) {
		printf("sending now ...%i\n", i);
		snprintf(buf, 1000, "Here I am: Count %i", i);
		// retcode = write(sockfd, "HERE I AM", 9);
		retcode = write(sockfd, buf, strlen(buf));
		if (retcode < 0) {
			perror("retcode");
		}
		sleep(1);
	}
	printf("Exiting\n");
	return 0;
}

