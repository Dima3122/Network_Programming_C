#include "foo.h"

int receive_data(int fd, struct sockaddr_in client_addr)
{
    int bytes_read, n;
    bytes_read = recv(fd, &n, sizeof(int), 0);
    if (bytes_read < 0)
    {
        perror("recv");
        exit(SOCKET_RECV_ERROR);
    }
    else if (bytes_read == 0)
    {
        return 1;
    }
    printf("Client address: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Received n = %d\n", n);
    return 0;
}

int main()
{
    int sock_client, sock_server,fd;
    socklen_t length;
    struct sockaddr_in server_addr, tmp_addr, client_addr[FD_SETSIZE];
    fd_set readfds, activefds;

    sock_server = Socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = 0;

    Bind(sock_server, (struct sockaddr *)&server_addr, sizeof(server_addr)); 
    length = sizeof(server_addr);
    Getsockname(sock_server, (struct sockaddr *)&server_addr, &length);
    printf("port number - %d\n", ntohs(server_addr.sin_port));
    
    Listen(sock_server, MAX_QUEUE);
    FD_ZERO(&activefds);
    FD_SET(sock_server, &activefds);
    while (1)
    {
        memcpy(&readfds, &activefds, sizeof(readfds));
        Select(FD_SETSIZE, &readfds, NULL, NULL, NULL); 
        if (FD_ISSET(sock_server, &readfds))
        {
            sock_client = Accept(sock_server, (struct sockaddr *)&tmp_addr, &length);
            memcpy(&client_addr[sock_client], &tmp_addr, sizeof(client_addr));
            FD_SET(sock_client, &activefds);
        }

        for (fd = 0; fd < FD_SETSIZE; fd++)
        {
            if (fd != sock_server && FD_ISSET(fd, &readfds))
            {
                if (receive_data(fd, client_addr[fd]))
                {
                    close(fd);
                    FD_CLR(fd, &activefds);
                    memset(&client_addr[fd], 0, sizeof(sizeof(struct sockaddr_in)));
                }
            }
        }
    }
    close(sock_server);
    return 0;
}