#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

//CGI 服务器：通过把服务器本地标准输入,输出或者文件重定向到网络连接中,
//这样我们就能够通过向标准输入,输出缓冲区中发送信息,达到在网络连接中发送信息的效果
int main(int argc, char *argv[])
{
    const char *ip = argv[0];
    int port = atoi(argv[1]);
    int backlog = atoi(argv[2]);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    addr.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0); //成功返回文件描述符，失败返回-1

    int ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret != -1); //成功返回0，失败返回-1

    int ret = listen(sock, backlog);
    assert(ret != -1); //成功返回0，失败返回-1

    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);

    int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlen);
    if (connfd < 0)
        //当函数发生异常时,一般会将errno变量赋一个整数值,不同的值表示不同的含义
        printf("errno is : %d\n", errno);
    else
    {
        close(STDOUT_FILENO);
        dup(connfd); //给原有文件描述符创建一个新的文件描述符
        printf("I CAN SAY IN THE INTERNET\n");
    }
    close(sock);
    return 0;
}