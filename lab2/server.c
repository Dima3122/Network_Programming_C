#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
// TCP
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
    while (1)
    {
        nread = recv(fd, buf, 1024, 0);
        if (nread <= 0)
        {
            break;
        }
        int status;
        pid_t pid = fork();
        if (pid == 0)
        {
            send(fd, buf, nread, 0);
        }
        else
        {
            do
            {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    close(fd);
    close(server);
    return 0;
}