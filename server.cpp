#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#include "utils.h"

#define MAX_EVENTS 1024

void setnonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    error_info(flags == -1, "fcntl get error");
    error_info(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1, "fcntl set error");
}

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
        bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1,
        "socket bind error"
    );
    
    error_info(
        listen(server_fd, SOMAXCONN) == -1,
        "socket listen error"
    );

    int epfd = epoll_create1(0);
    error_info(epfd == -1, "epoll create error");
    struct epoll_event events[MAX_EVENTS], ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

    
    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        error_info(nfds == -1, "epoll wait error");
        for (int i = 0;i < nfds;++i) {
            if (events[i].data.fd == server_fd) {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                bzero((char*)&client_addr, sizeof(client_addr));
                int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
                printf("client fd %d connected! IP: %s, Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                ev.data.fd = client_fd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(client_fd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
            }
            else if (events[i].events & EPOLLIN) {
                char buffer[256];
                bzero(buffer, sizeof(buffer));
                while (true) {
                    ssize_t n = read(events[i].data.fd, buffer, sizeof(buffer));
                    if (n > 0) {
                        printf("message from client fd %d: %s\n", events[i].data.fd, buffer);
                        write(events[i].data.fd, buffer, sizeof(buffer));
                    }
                    else if (n == -1 && errno == EINTR) {
                        printf("continue reading\n");
                        continue;
                    }
                    else if (n== -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                        printf("no more data\n");
                        break;
                    }
                    else {
                        printf("client fd %d closed\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }
                }
            }
            else {
                printf("other event\n");
            }
        }
    }
    close(server_fd);
    return 0;
}