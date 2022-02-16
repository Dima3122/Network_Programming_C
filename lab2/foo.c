#include "foo.h"

int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if (res == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Getsockname(int sockfd, struct sockaddr *addr, socklen_t *len)
{
    if (getsockname(sockfd, (struct sockaddr *)addr, len) < 0)
    {
        perror("getsockname");
        exit(SOCKET_GETSOCKNAME_ERROR);
    }
}

void Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, timeval *timeout)
{
    if (select(FD_SETSIZE, readfds, NULL, NULL, NULL) < 0)
    {
        perror("select");
        exit(SELECT_ERROR);
    }
}
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = bind(sockfd, addr, addrlen);
    if (res == -1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog)
{
    int res = listen(sockfd, backlog);
    if (res == -1)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int res = accept(sockfd, addr, addrlen);
    if (res == -1)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = connect(sockfd, addr, addrlen);
    if (res == -1)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(int af, const char *src, void *dst)
{
    int res = inet_pton(af, src, dst);
    if (res == 0)
    {
        printf("inet_pton failed: src does not contain a character"
               " string representing a valid network address in the specified"
               " address family\n");
        exit(EXIT_FAILURE);
    }
    if (res == -1)
    {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
}
