#ifndef SOCKET_API_H
#define SOCKET_API_H

struct addrinfo get_hints(int socktype, int family, int protocol, int flags);

int raise_connection(int *sockfd);

#endif