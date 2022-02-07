#include <arpa/inet.h>
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "foo.h"

#define MAX_QUEUE 10

int main()
{
    signal(SIGCHLD, SIG_IGN); // после завершения процессы не преобразовываются в зомби
    int sock_client, sock_server;
    socklen_t length;
    struct sockaddr_in server_addr, client_addr;
    int n;
    pid_t pid;

    sock_server = Socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = 0;
    Bind(sock_server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    length = sizeof(server_addr);
    if (getsockname(sock_server, (struct sockaddr *)&server_addr, &length) < 0)
    {
        perror("getsockname");
        exit(3);
    }
    printf("SERVER: port number - %d\n", ntohs(server_addr.sin_port));
    Listen(sock_server, MAX_QUEUE);
    while (1)
    {
        sock_client = Accept(sock_server, (struct sockaddr *)&client_addr, &length);
        pid = fork();
        switch (pid)
        {
        case 0:
            close(sock_server);
            while (1)
            {
                if ((recv(sock_client, &n, sizeof(int), 0)) <= 0)
                {
                    break;
                }
                printf("SERVER: Client address: %s:%d\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                printf("SERVER: Received n = %d\n", n);
            }
            close(sock_client);
            exit(EXIT_SUCCESS);
            break;
        case -1:
            perror("fork");
            exit(5);
            break;
        default:
            close(sock_client);
            break;
        }
    }
    close(sock_server);
    return 0;
}