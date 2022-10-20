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
    } else {
        printf("connnected\n");
    }

    // send to server and recieve
    char message[100] = {};
    int str_len;
    FILE* writefp = fdopen(client_sock, "w");
    FILE* readfp = fdopen(client_sock, "r");
    while(1){
        // scanf("%s", message);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q")){
            break;
        }
        
        // str_len = write(client_sock, message, strlen(message));
        int tmp = fputs(message, writefp);
        // printf("%d\n", tmp);
        fflush(writefp);
        fgets(message, sizeof(message), readfp);
        // int recieve_len = 0;
        // while(recieve_len < str_len){
        //     int recieve_cnt = read(client_sock, &message[recieve_len], sizeof(message) - 1);
        //     if(recieve_cnt == -1){
        //         error_handling("error read\n");
        //     }
        //     recieve_len += recieve_cnt;
        // }
        // message[str_len] = '\0';
        printf("messege from server : %s\n", message);
    }

    close(client_sock);
    return 0;
}