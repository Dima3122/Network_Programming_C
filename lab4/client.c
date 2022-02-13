#include <arpa/inet.h>
#include <memory.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "foo.h"
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server_addr;
    struct hostent *hp;
    int i, n;

    if (argc < 4)
    {
        printf("ENTER ./bin/client hostname port n\n");
        exit(1);
    }
    sock = Socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    bcopy(hp->h_addr, &server_addr.sin_addr, hp->h_length);
    server_addr.sin_port = htons(atoi(argv[2]));

    Connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    printf("CLIENT: Is ready for sending\n");
    printf("CLIENT: Sending...\n");
    n = atoi(argv[3]);
    for (i = 0; i < n; i++)
    {
        send(sock, &n, sizeof(int), 0);
        printf("CLIENT: Sent %d from %d\n", i + 1, n);
        sleep(n);
    }

    printf("CLIENT: Sending complete\n");
    close(sock);

    return 0;
}