#include<cstdio>
#include<cstdlib> // atoi
#include<string.h> // memset
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
constexpr int BUF_SIZE = 1024;
constexpr int SMALL_BUF = 100;

void* request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(const char* p);

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
    pthread_t t_id;
    while(1){
        int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_addr_size);
        printf("connnected to IP: %s client_sock is: %d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pthread_create(&t_id, NULL, request_handler, (void*)&client_sock);
        pthread_detach(t_id);
    }
    close(server_sock);
    return 0;
}
void* request_handler(void* arg) {
    int client_sock = *((int*)arg);
    char req_line[SMALL_BUF];
    FILE* client_read = fdopen(client_sock, "r");
    FILE* client_write = fdopen(dup(client_sock), "w");
    fgets(req_line, SMALL_BUF, client_read);

    if(strstr(req_line, "HTTP/1.1") == NULL) {
        send_error(client_write);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }

    char method[10];
    char file_name[30];
    char ct[15];
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));

    if(strcmp(method, "GET") != 0) {
        send_error(client_write);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }
    fclose(client_read);
    send_data(client_write, ct, file_name);
}

void send_data(FILE* fp, char* ct, char* file_name) {
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE* send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");
    if(send_file == NULL) {
        printf("no file\n");
        send_error(fp);
        return;
    }
    printf("has file\n");
    // 传输头信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    // 传输请求数据
    while(fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

char* content_type(char* file) {
    // 解析文件后缀名 .html 或者 .htm
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));
    if(!strcmp(extension, "html") || !strcmp(extension, "htm")){
        return "text/html";
    } else {
        return "text/plain";
    }
}

void send_error(FILE* fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                    "<body><font size=+5><br> error happen! check request name and request way! "
                    "</font></body><html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fflush(fp);
}
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
}