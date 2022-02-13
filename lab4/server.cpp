#include <arpa/inet.h>
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <algorithm>
#include <sys/time.h>
#include <fcntl.h>
#include <set>
#include "foo.h"

using namespace std;

#define MAX_QUEUE 10

int main()
{
    signal(SIGCHLD, SIG_IGN); // после завершения процессы не преобразовываются в зомби
    int sock_server;
    socklen_t length;
    struct sockaddr_in server_addr;
    int bytes_read = 0;
    char buf[1024];

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
    set<int> clients;
    clients.clear();
    printf("SERVER: port number - %d\n", ntohs(server_addr.sin_port)); //не работает дальше, такое ощущение что вообще дохнет
    printf("я тут");
    Listen(sock_server, MAX_QUEUE);
    while (1)
    {
        // Заполняем множество сокетов
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(sock_server, &readset);

        for (set<int>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            FD_SET(*it, &readset);
        }

        // Задаём таймаут
        timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        // Ждём события в одном из сокетов
        int mx = max(sock_server, *max_element(clients.begin(), clients.end()));
        if (select(mx + 1, &readset, NULL, NULL, &timeout) <= 0)
        {
            perror("select");
            exit(3);
        }

        // Определяем тип события и выполняем соответствующие действия
        if (FD_ISSET(sock_server, &readset))
        {
            // Поступил новый запрос на соединение, используем accept
            int sock = Accept(sock_server, NULL, NULL);
            if (sock < 0)
            {
                perror("accept");
                exit(3);
            }

            fcntl(sock, F_SETFL, O_NONBLOCK);

            clients.insert(sock);
        }

        for (set<int>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if (FD_ISSET(*it, &readset))
            {
                // Поступили данные от клиента, читаем их
                bytes_read = recv(*it, buf, 1024, 0);

                if (bytes_read <= 0)
                {
                    // Соединение разорвано, удаляем сокет из множества
                    close(*it);
                    clients.erase(*it);
                    continue;
                }
                printf("buf");
                // Отправляем данные обратно клиенту
                send(*it, buf, bytes_read, 0);
            }
        }
    }
    close(sock_server);
    return 0;
}