#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#include "utils.h"

int main() {
    /**
     * @param Address Family: (IPv4: AF_INET , IPv6: AF_INET6)
     * @param Data Transmission Type: (TCP: SOCK_STREAM , UDP: SOCK_DGRAM)
     * @param Protocol: 0 (Default Protocol), (TCP: IPPROTO_TCP , UDP: IPPROTO_UDP)
     * @return File Descriptors
     */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    error_info(server_fd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    error_info(
    bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1,
    "socket bind error"
    );
    
    error_info(
    listen(server_fd, SOMAXCONN) == -1,
    "socket listen error"
    );

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero((char *) &client_addr, sizeof(client_addr));
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    error_info(client_fd == -1, "socket accept error");
    printf("Client Connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while(true) {
        char buffer[256];
        bzero(buffer, sizeof(buffer));
        ssize_t n = read(client_fd, buffer, sizeof(buffer));
        if(n > 0){
            printf("message from client fd %d: %s\n", client_fd, buffer);
            write(client_fd, buffer, sizeof(buffer));
        }
        else if (n == 0) {
            printf("Client fd %d disconnected\n", client_fd);
            close(client_fd);
            break;
        }
        else {
            close(client_fd);
            error_info(true, "socket read error");
            break;
        }
    }
}