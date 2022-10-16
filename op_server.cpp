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
int cal(int oprand_cnt, int* message, char op);
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

        // read oprand_cnt, 需要先读取操作数的个数，方便后面读取操作数
        int oprand_cnt;
        read(client_sock, &oprand_cnt, 1);
        // 读取操作数和操作符
        int recieve_len = 0;
        while(recieve_len < oprand_cnt * 4 + 1){
            int recieve_cnt = read(client_sock, &message[recieve_len], sizeof(message));
            if(recieve_cnt == -1){
                error_handling("error read\n");
            }
            recieve_len += recieve_cnt;
        }
        int res = cal(oprand_cnt, (int*) message, message[oprand_cnt * 4]);
        write(client_sock, &res, 4);
        close(client_sock);
    }
    close(server_sock);
    return 0;
}
int cal(int oprand_cnt, int* message, char op){
    int res = message[0];
    for(int i = 1; i < oprand_cnt; ++i){
        switch (op) {
            case '+':
                res += message[i];
                break;
            case '-':
                res -= message[i];
                break;
            case '*':
                res *= message[i];
                break;
            default:
                break;
        }
    }
    return res;
}