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
    int server_sock = socket(PF_INET, SOCK_DGRAM, 0);
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


    // recv and send
    socklen_t client_addr_size;
    sockaddr_in client_addr;
    int str_len;
    char message[100] = {};
    while(1){
        sleep(1);
        str_len = recvfrom(server_sock, message, sizeof(message), 0, (sockaddr*)&client_addr, &client_addr_size); 
        printf("recive : %d %s\n", str_len, message);
        sendto(server_sock, message, str_len, 0, (sockaddr*)&client_addr, client_addr_size);
        printf("already send\n");
    }
    close(server_sock);
    return 0;
}