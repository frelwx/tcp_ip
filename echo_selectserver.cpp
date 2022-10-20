#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<unistd.h>
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
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

    // select
    fd_set reads, tmps;
    FD_ZERO(&reads);
    FD_SET(server_sock, &reads);// 监视 服务器监听套接字
    int fd_max = server_sock;
    timeval timeout;

    socklen_t client_addr_size;
    sockaddr_in client_addr;
    int str_len;
    char message[100] = {};
    while(1){
        tmps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int res = select(fd_max + 1, &tmps, 0, 0, &timeout);

        if(res == -1) {
            error_handling("select failed\n");
        } else if(res == 0) {
            printf("timeout\n");
            continue;
        } else {
            for(int i = 0; i < fd_max + 1; ++i) {
                if(FD_ISSET(i, &tmps)){
                    if(i == server_sock){ // this is a connection requese ,need to accept
                        int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);
                        FD_SET(client_sock, &reads);
                        fd_max = fd_max > client_sock ? fd_max : client_sock;
                        printf("new client connected %d \n", client_sock);
                    } else {
                        while((str_len = read(i, message, sizeof(message))) != 0){
                            printf("message len %d\n", str_len);
                            write(i, message, str_len);
                        }
                        printf("end\n");
                        FD_CLR(i, &reads);
                        close(i);
                        printf("client %d closed\n", i);
                    }
                }
            }
        }
    }
    close(server_sock);
    return 0;
}
