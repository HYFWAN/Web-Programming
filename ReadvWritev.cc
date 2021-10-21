#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>
#define BUFFER_SIZE 1024

static char *file_name = "file";
static const char *status_line[2] = {"200,OK", "500 Internal server error"};
int main(int argc, char *argv[])
{
    const char *ip = argv[0];
    int port = atoi(argv[1]);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);

    int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlen);
    if (connfd < 0)
        printf("errno is %d\n", errno);
    else
    {
        //保存HTTP应答的状态行，头部字段和一个空行的缓存区
        char header_buf[BUFFER_SIZE];
        memset(header_buf, '\0', BUFFER_SIZE);
        //存放目标文件内容的应用程序缓存
        char *file_buf;
        //获取目标文件的属性，例如是否为目录，文件的大小等
        struct stat file_stat;
        //获取目标文件是否是有效文件
        bool valid = true;
        //缓存区header_buf目前已经使用年多少字节空间
        int len = 0;
        if (stat(file_name, &file_stat) < 0) //用于获取文件信息，成功返回0，返回-1代表目标文件不存在
            valid = false;
        else
        {
            if (S_ISDIR(file_stat.st_mode))
                valid = false;
            else if (file_stat.st_mode & S_IROTH) //当前用户是否有读取文件的权限
            {
                int fd = open(file_name, O_RDONLY);
                file_buf = new char[file_stat.st_size + 1];
                memset(file_buf, '\0', file_stat.st_size + 1);
                if (read(fd, file_buf, file_stat.st_size) < 0)
                    valid = false;
            }
            else
                valid = false;
        }
        if (valid) //如果目标文件有效，则发送正常的HTTP应答
        {
            //将HTTP应答的状态行，头部字段和一个空行依次加入到header_buf
            ret = snprintf(header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1", status_line[0]);
            len += ret;

            ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len, "Content-Length: %d\r\n", file_stat.st_size);
            len += ret;

            ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len, "%s", "\r\n");

            //利用writev将header_buf和file_buf的内容一并写出
            struct iovec iv[2];
            iv[0].iov_base = header_buf;
            iv[0].iov_len = strlen(header_buf);
            iv[1].iov_base = file_buf;
            iv[1].iov_len = file_stat.st_size;
            ret = writev(connfd, iv, 2);
        }
        else
        { //目标文件无效，通知客户端服务器发生年“内部错误”
            ret = snprintf(header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1", status_line[1]);
            send(connfd, header_buf, strlen(header_buf), 0);
        }
        close(connfd);
        delete[] file_buf;
    }
    close(sock);
    return 0;
}