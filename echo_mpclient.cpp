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
void read_routine(int client_sock, char* message){
    while(1){
        int str_len = read(client_sock, message, 100);
        message[str_len] = '\0';
        printf("message from server %s %d\n", message, str_len);
    }
}
void write_routine(int client_sock, char* message){
    while(1){
        scanf("%s", message);
        if(!strcmp(message, "q")){
            break;
        }
        write(client_sock, message, strlen(message));
    }
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
    } else {
        printf("connnected\n");
    }

    // send to server and recieve
    char message[100] = {};
    pid_t pid = fork();
    
    if(pid == 0) {
        write_routine(client_sock, message);
        printf("end wite\n");
        close(client_sock);
        return 0;
    } else {
        printf("son pid%d\n", pid);
        read_routine(client_sock, message);
        close(client_sock);
        return 0;
    }
}