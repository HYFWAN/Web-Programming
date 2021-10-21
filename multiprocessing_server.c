#include <stdio.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <signal.h>

#define SER_PORT 9999
#define BUF_SIZE 10
#define SER_IP "10.33.219.65"

void catch_child()
{
    while (waitpid(0, NULL, WNOHANG) > 0)
    {
    };
    return;
}

int main()
{
    int lfd, cfd;
    pid_t pid;
    socklen_t clt_addr_len;
    struct sockaddr_in ser_addr;
    struct sockaddr_in clt_addr;

    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SER_PORT);
    //ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, SER_IP, &ser_addr.sin_addr.s_addr);

    lfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(lfd >= 0);

    int ret = bind(lfd, (const struct sockaddr *)&ser_addr, (socklen_t)sizeof(ser_addr));
    assert(ret != -1);

    ret = listen(lfd, 5);
    assert(ret != -1);

    clt_addr_len = sizeof(clt_addr);
    while (1)
    {
        cfd = accept(lfd, (struct sockaddr *)&clt_addr, &clt_addr_len);
        assert(ret != -1);

        pid = fork();

        if (pid < 0)
        {
            perror("ford error");
            exit(1);
        }
        else if (pid == 0)
        {
            close(lfd);
            break;
        }
        else
        {
            struct sigaction act;
            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            int r = sigaction(SIGCHLD, &act, NULL);
            if (r != 0)
            {
                perror("sigaction error");
                exit(1);
            }
            close(cfd);
            continue;
        }
    }

    if (pid == 0)
    {
        char buf[BUF_SIZE];
        int r = read(cfd, &buf, BUF_SIZE);
        if (r == 0)
        {
            close(cfd);
            exit(0);
        }
        int i = 0;
        for (; i < BUF_SIZE; i++)
            buf[i] = toupper(buf[i]);
        write(cfd, buf, r);
        write(STDOUT_FILENO, buf, r);
    }

    return 0;
}