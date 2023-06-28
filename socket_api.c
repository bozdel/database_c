#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define BACKLOG 5

struct addrinfo get_hints(int socktype, int family, int protocol, int flags) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = socktype;
	hints.ai_family = family;
	hints.ai_protocol = protocol;
	hints.ai_flags = flags;
	return hints;
}

void print_addrinfo(struct addrinfo ainfo) {
	printf("family: ");
	if (ainfo.ai_family == AF_INET) {
		printf("AF_INET\n");
	}
	else if (ainfo.ai_family == AF_INET6) {
		printf("AF_INET6\n");
	}
	else if (ainfo.ai_family == AF_UNSPEC) {
		printf("AF_UNSPEC\n");
	}
	printf("socktype: ");
	if (ainfo.ai_socktype == SOCK_STREAM) {
		printf("SOCK_STREAM\n");
	}
	else if (ainfo.ai_socktype == SOCK_DGRAM) {
		printf("SOCK_DGRAM\n");
	}
	printf("protocol: %d\n", ainfo.ai_protocol);
	printf("flags: %d\n", ainfo.ai_flags);
	printf("canonname: %s\n", ainfo.ai_canonname);
	printf("addrlen: %d\n", ainfo.ai_addrlen);
}

int raise_connection(int *sockfd) {
	// int sockfd;
	struct addrinfo *ainfol = NULL; // pointer to addres info list
	struct addrinfo hints = get_hints(SOCK_STREAM, AF_INET, 0, AI_PASSIVE);

	int err = getaddrinfo(NULL, "http-alt", &hints, &ainfol);
	if (err != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
	}

	bool bound = false;
	int i = 0;
	for (struct addrinfo *ainfo = ainfol; ainfo != NULL; ainfo = ainfo->ai_next) {
		printf("[%d]: ", i);
		i++;
		print_addrinfo(*ainfo);
		*sockfd = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);
		if (*sockfd == -1) {
			printf("no sock\n");
			continue;
		}
		if (bind(*sockfd, ainfo->ai_addr, ainfo->ai_addrlen) == 0) {
			printf("bound\n");
			if (listen(*sockfd, BACKLOG) == -1) {
				perror("listen");
			}
			else {
				printf("listening\n");
				bound = true;
			}
			break;
		}
		else {
			perror("bind");
		}
		close(*sockfd);
	}

	if (!bound) {
		printf("not bound\n");
		return 0;
	}
	return 1;
}