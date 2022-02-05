#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

char *concat(char *s1, char *s2)
{

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char *result = malloc(len1 + len2 + 1);

    if (!result)
    {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);

    return result;
}

int main()
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    bind(server, (struct sockaddr *)&adr, sizeof adr);
    listen(server, 5);
    socklen_t adrlen = sizeof adr;
    int fd = accept(server, (struct sockaddr *)&adr, &adrlen);
    ssize_t nread;
    char buf[256];
    char buff[256];
    nread = read(fd, buf, 256);
    if (nread == -1)
    {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    if (nread == 0)
    {
        printf("END OF FILE occured\n");
    }
    write(STDOUT_FILENO, buf, nread);
    char *c = concat(buf, "world");
    write(fd, c, nread + 6);
    inet_ntop(AF_INET, &adr.sin_addr.s_addr, buff, sizeof(buff));
    write(STDOUT_FILENO, buff, 10);
    while (1)
    {
        nread = recv(fd, buf, 1024, 0);
        if (nread <= 0)
        {
            break;
        }
        send(fd, buf, nread, 0);
    }
    sleep(5);
    close(fd);
    close(server);
    return 0;
}