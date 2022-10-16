#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<arpa/inet.h>
#include<sys/socket.h>
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

    // accept
    socklen_t client_addr_size;
    sockaddr_in client_addr;
    int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);
    printf("%d %d %d\n", ntohs(client_addr.sin_port), ntohs(server_addr.sin_port), client_addr_size);
    if(client_sock == -1){
        error_handling("accept error\n");
    }

    // write to client
    char message[] = "hello world!";
    write(client_sock, message, sizeof(message));
    close(client_sock);
    close(server_sock);
    return 0;
}