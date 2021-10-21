#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    //服务器地址
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    //绑定
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address) < 0))
        printf("connection failed\n");
    else
    {
        const char *oob_data = "abc";
        const char *normal_data = "123";
        send(sockfd, oob_data, strlen(oob_data), 0);
        send(sockfd, normal_data, strlen(normal_data), MSG_OOB);
        send(sockfd, normal_data, strlen(normal_data), 0);
    }
    close(sockfd);
    return 0;
}