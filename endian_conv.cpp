#include<cstdio>
#include<arpa/inet.h>
int main(int argc, char* argv[]){
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;
    net_port = htons(host_port);
    net_addr = htonl(host_addr);
    // printf 控制的字符串格式 %[标志][最小宽度][.精度][类型长度]类型。 x表示16进制类型 #表示在前面输出0x 类型长度有h（short）和l （long）
    printf("host order port %#hx\n", host_port);
    printf("net order port %#hx\n", net_port);
    printf("host order address %#lx\n", host_addr);
    printf("net order address %#lx\n", net_addr);
    return 0;
}
// host order port 0x1234
// net order port 0x3412
// host order address 0x12345678
// net order address 0x78563412
// 对于0x12345678来说12是一个字节、34是一个字节