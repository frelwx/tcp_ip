#include<cstdio>
#include<iostream>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
using namespace std;
int main(){
    // 定义套接字
    auto serv_socket = socket(PF_INET, SOCK_STREAM, 0);// 第三个参数位0，默认选择对应的协议
    
    // 初始化地址
    sockaddr_in serv_addr;
    char* serv_port = "9190";
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(serv_port));

    // 向socket分配地址
    bind(serv_socket, (sockaddr*) &serv_addr, sizeof(serv_addr));
}



