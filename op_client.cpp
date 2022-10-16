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
    // 操作数数量
    int oprand_cnt;
    scanf("%d", &oprand_cnt);
    message[0] = char(oprand_cnt);
    write(client_sock, message, 1);
    // 操作数
    for(int i = 0; i < oprand_cnt; ++i){
        scanf("%d", (int*)&message[i * 4 + 1]);
    }
    write(client_sock, &message[1], oprand_cnt * 4);

    // 操作符
    scanf("%c", &message[oprand_cnt * 4+ 1]); // 多读一次，过滤回车
    scanf("%c", &message[oprand_cnt * 4+ 1]);
    write(client_sock, &message[oprand_cnt * 4 + 1], 1);

    // get ans
    char res[4];
    int recieve_len = 0;
    while(recieve_len < 4){
        int recieve_cnt = read(client_sock, res, 4);
        if(recieve_cnt == -1){
            error_handling("error read\n");
        }
        recieve_len += recieve_cnt;
    }
    printf("%d\n", *(int*)res);
    close(client_sock);
    return 0;
}