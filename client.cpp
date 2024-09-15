#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "utils.h"

int main ()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    error_info(sockfd == -1, "socket create error");
    
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    error_info(
        connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1,
        "socket connect error"
    );
    
    while (true)
    {
        char buffer[256];
        bzero(buffer, sizeof(buffer));
        scanf("%s", buffer);
        ssize_t n = write(sockfd, buffer, sizeof(buffer));
        printf("write_bytes = %ld\n", n);
        if (n == -1){
            printf("socket already closed\n");
            break;
        }
        bzero(buffer, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer));
        printf("read_bytes = %ld\n", n);
        if (n > 0){
            printf("message from server: %s\n", buffer);
        }
        else if (n == 0){
            printf("server disconnected\n");
            break;
        }
        else {
            close(sockfd);
            error_info(true, "socket read error");
            break;
        }
    }
    close(sockfd);
    return 0;
}