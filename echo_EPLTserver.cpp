#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/time.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<memory>
constexpr int EPOLL_SIZE = 30;
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
}
void set_nonblocking(int fd) {
    int flag = fcntl(fd,F_GETFL,0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}
int main(int argc, char *argv[]){
    // create socket    socket()
    int server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(server_sock == -1){
        error_handling("error socket\n");
    }
    // allocate address and ip      bind()
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    if(bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        error_handling("error bind\n");
    }

    // listen()
    if(listen(server_sock, 5) == -1){
        error_handling("listen error\n");
    }

    // create epoll fd and server_sock to epfd
    int epfd = epoll_create(EPOLL_SIZE);
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_sock, &event);


    epoll_event* ep_events = new epoll_event[EPOLL_SIZE];
    socklen_t client_addr_size;
    sockaddr_in client_addr;
    int str_len;
    char message[4] = {};
    set_nonblocking(server_sock);
    while(1){
        int event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        printf("return epoll_wait\n");
        if(event_cnt == -1) {
            error_handling("error epoll wait\n");
        }
        for(int i = 0; i < event_cnt; ++i) {
            if(ep_events[i].data.fd == server_sock) {
                int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);
                set_nonblocking(client_sock);
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);
                printf("connnected: %d \n", client_sock);
            } else {
                while(1) {
                    str_len = read(ep_events[i].data.fd, message, sizeof(message));
                    if(str_len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed %d\n", ep_events[i].data.fd); // 这里忘记加\n，导致没有刷新缓冲区，下次printf才能显示这句话
                        break;
                    } else if(str_len < 0) {
                        if(errno == EAGAIN) {
                            printf("empty buf\n");
                            break;
                        }
                    } else {
                        message[str_len] = '\0';
                        printf("event_cnt: %d i: %d\n", event_cnt, i);
                        printf("recv: %s\n", message);
                        write(ep_events[i].data.fd, message, str_len);
                    }
                }
            }
        }
    }
    close(epfd);
    close(server_sock);
    return 0;
}
