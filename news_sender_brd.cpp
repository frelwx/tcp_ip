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

    // set TTL
    int so_brd = 1;
    setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, (void*) &so_brd, sizeof(so_brd));

    // multicast
    FILE* fp;
    char buf[100] = {};
    if(!(fp = fopen("news.txt", "r"))) {
        error_handling("error open news.txt\n");
    }
    while(!feof(fp)) {
        fgets(buf, sizeof(buf), fp);
        sendto(client_sock, buf, strlen(buf), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        sleep(2);
    }
    fclose(fp);
    close(client_sock);
    return 0;
}