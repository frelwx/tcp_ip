#include<cstdio>
#include<arpa/inet.h>
int main(){
    char *addr1 = "1.2.3.4";
    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE){
        printf("error\n");
    } else {
        printf("%#lx\n", conv_addr);
    }
    return 0;
}