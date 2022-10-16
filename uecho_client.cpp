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
    int client_sock = socket(PF_INET, SOCK_DGRAM, 0);
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

    connect(client_sock, (sockaddr*)&server_addr, sizeof(server_addr));
    // send to server and recieve
    char message[100] = {};
    int str_len;
    sockaddr_in from_addr;
    socklen_t from_addr_size;
    while(1){
        scanf("%s", message);
        if(!strcmp(message, "q")){
            break;
        }

        // sendto(client_sock, message, strlen(message), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        write(client_sock, message, strlen(message));
        printf("already send\n");
        // str_len = recvfrom(client_sock, message, sizeof(message), 0, (sockaddr*)&from_addr, &from_addr_size);
        str_len = read(client_sock, message, sizeof(message));
        printf("%d", str_len);
        if(str_len == -1){
            error_handling("error recvfrom\n");
        }
        message[str_len] = '\0';
        printf("messege from server : %s\n", message);
    }

    close(client_sock);
    return 0;
}