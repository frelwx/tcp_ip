#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/wait.h>
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
}
void read_childproc(int sig){
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        if(WIFEXITED(status)){
            printf("remove child id %d\n", pid);
            printf("child send %d\n", WEXITSTATUS(status));
        }
    }
}
int main(int argc, char *argv[]){
    // register signal function
    signal(SIGCHLD, read_childproc);
    // struct sigaction act;
    // act.sa_handler = read_childproc;
    // sigemptyset(&act.sa_mask);
    // act.sa_flags = 0;
    // int state = sigaction(SIGCHLD, &act, 0);
    // if(state == -1){
    //     printf("error sigaction\n");
    // } else {
    //     printf("%d\n", state);
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
    int strlen;
    char message[100] = {};
    while(1){
        int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);
        if(client_sock == -1){
            printf("accept error\n");
            continue;
        } else {
            printf("connnect to %d!\n", client_sock);
        }
        pid_t pid = fork();
        if(pid == -1){
            close(client_sock);
            printf("error fork\n");
            continue;
        }
        if(pid == 0){ 
            // 子线程
            while((strlen = read(client_sock, message, sizeof(message))) != 0){
                write(client_sock, message, strlen);
            }
            close(client_sock);
            printf("disconnected client sock\n");
            close(server_sock);
            return 0;
        } else {
            // 父线程，当前客户端的服务结束后，需要关闭相应的套机字，这里有一个类似引用计数的思想。不同进程都需要关闭。
            close(client_sock);
        }

        close(client_sock);
    }
    close(server_sock);
    return 0;
}