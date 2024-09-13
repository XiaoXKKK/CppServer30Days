#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>

int main() {
    /**
     * @param Address Family: (IPv4: AF_INET , IPv6: AF_INET6)
     * @param Data Transmission Type: (TCP: SOCK_STREAM , UDP: SOCK_DGRAM)
     * @param Protocol: 0 (Default Protocol), (TCP: IPPROTO_TCP , UDP: IPPROTO_UDP)
     * @return File Descriptors
     */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    
    listen(server_fd, SOMAXCONN);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero((char *) &client_addr, sizeof(client_addr));
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    printf("Client Connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}