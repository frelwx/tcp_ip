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
    int strlen;
    char message[100] = {};
    for(int i = 0; i < 5; ++i){
        int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);
        if(client_sock == -1){
            error_handling("accept error\n");
        } else {
            printf("connnect to %d!\n", i + 1);
        }
        FILE* readfp = fdopen(client_sock, "r");
        FILE* writefp = fdopen(client_sock, "w");
        while(!feof(readfp)) {
            fgets(message, sizeof(message), readfp);
            // printf("receive %s\n", message);
            fputs(message, writefp);
            fflush(writefp);
        }
        while((strlen = read(client_sock, message, sizeof(message))) != 0){
            write(client_sock, message, strlen);
        }
        close(client_sock);
    }
    close(server_sock);
    return 0;
}