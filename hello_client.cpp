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
    int client_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(client_sock == -1){
        error_handling("sock error\n");
    }

    // define server IP and Port
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    inet_aton(argv[1], &server_addr.sin_addr);
    server_addr.sin_port = htons(atoi(argv[2]));
    if(connect(client_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        error_handling("error connect\n");
    }

    // read 
    char message[30];
    int str_len = read(client_sock, message, sizeof(message) - 1);
    if(str_len == -1){
        error_handling("read error\n");
    }
    // end
    printf("%s\n", message);
    close(client_sock);
    return 0;
}