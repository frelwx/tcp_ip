#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/time.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
}
void send_msg(char* msg, int len);
void* handle_client(void* arg);

int client_cnt = 0;
int client_sock_s[256];
pthread_mutex_t mutex;
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


    socklen_t client_addr_size;
    sockaddr_in client_addr;
    int str_len;
    char message[4] = {};
    pthread_mutex_init(&mutex, NULL);
    pthread_t t_id;
    while(1){
        int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);
        pthread_mutex_lock(&mutex);
        client_sock_s[client_cnt++] = client_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_client, (void*)&client_sock);
        pthread_detach(t_id);
        printf("connnected to IP: %s client_sock is: %d \n", inet_ntoa(client_addr.sin_addr), client_sock);
    }
    close(server_sock);
    return 0;
}
void* handle_client(void* arg) {
    int client_sock = *((int*)arg);
    int str_len = 0;
    char msg[100] = {};
    while((str_len = read(client_sock, msg, sizeof(msg) - 1)) != 0) {
        send_msg(msg, str_len);
    }
    pthread_mutex_lock(&mutex);
    printf("all client_sock: ");
    for(int i = 0; i < client_cnt; ++i) {
        printf("(%d %d) ", i, client_sock_s[i]);
    }
    printf("\n");
    for(int i = 0; i < client_cnt; ++i) {
        if(client_sock == client_sock_s[i]) {
            printf("begin: %d %d\n", i, client_sock_s[i]);
            while(i < client_cnt - 1) {
                client_sock_s[i] = client_sock_s[i + 1];
                printf("%d %d\n", i, client_cnt);
                ++i;
            }
            break;
        }
    }
    --client_cnt;
    pthread_mutex_unlock(&mutex);
    close(client_sock);
    return NULL;
}
void send_msg(char* msg, int len) {
    pthread_mutex_lock(&mutex);
    for(int i = 0 ; i < client_cnt; ++i) {
        write(client_sock_s[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}