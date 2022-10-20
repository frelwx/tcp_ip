#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<netinet/in.h>
int client_sock;
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
}
void urg_handler(int sig){
    char buf[30] = {};
    int str_len = recv(client_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = '\0';
    printf("%d urgent message : %s \n", str_len, buf);
}
int main(int argc, char *argv[]){
    // register signal function
    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int state = sigaction(SIGURG, &act, 0);
    // if(state == -1){
    //     printf("error sigaction\n");
    // }
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
    client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);

    // set which progress to run handle function
    fcntl(client_sock, F_SETOWN, getpid());
    
    int str_len;
    char message[100] = {};
    while((str_len = recv(client_sock, message, sizeof(message), 0)) != 0) {
        if(str_len == -1) {
            continue;
        }
        message[str_len] = '\0';
        printf("%d recv nomal : %s\n", str_len, message);
    }

    close(client_sock);
    close(server_sock);
    return 0;
}