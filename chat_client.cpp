#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
}
void* send_msg(void* arg);
void* recv_msg(void* arg);

char name[20] = "[default]";
char msg[100];
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

    sprintf(name, "[%s]", argv[3]);

    pthread_t send_thread, recv_thread;
    void* thread_return;
    pthread_create(&send_thread, NULL, send_msg, (void*)&client_sock);
    pthread_create(&recv_thread, NULL, recv_msg, (void*)&client_sock);
    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);
    close(client_sock);
    return 0;
}
void* send_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[120];
    while(1) {
        scanf("%s", msg);
        if(!strcmp(msg, "q")){
            close(sock);
            exit(0);
            break;
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}
void* recv_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[120];
    int str_len;
    while(1) {
        str_len = read(sock, name_msg, sizeof(name_msg) - 1);
        if(str_len == -1) {
            return (void*)(-1);
        }
        name_msg[str_len] = '\0';
        printf("%s\n", name_msg);
    }
    return NULL;
}