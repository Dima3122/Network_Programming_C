#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    connect(fd, (struct sockaddr *)&adr, sizeof adr);
    int buf;
    int i = 0;
    printf("Введите i ");
    scanf("%d", &i);
    for (int j = 0; j < i; j++)
    {
        send(fd, &i, sizeof(i), 0);
        recv(fd, &buf, sizeof(i), 0);

        printf("%d ", buf);
        sleep(1);
    }
    printf("\n");
    sleep(2);
    close(fd);
    return 0;
}