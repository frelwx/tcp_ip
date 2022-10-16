#include<cstdio>
#include<iostream>
#include<arpa/inet.h>
using namespace std;
int main(){
    char *addr = "127.232.124.79";
    sockaddr_in addr_inet;
    std::cout << addr_inet.sin_family << sizeof(addr_inet.sin_family) << std::endl;
    if(!inet_aton(addr, &addr_inet.sin_addr)) {
        printf("error\n");
    } else {
        printf("%#x\n", addr_inet.sin_addr);
    }
    return 0;
}