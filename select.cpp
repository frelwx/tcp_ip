#include<cstdio>
#include<unistd.h>
#include<sys/select.h>
#include<sys/time.h>
#include<cstdlib>
void error_handling(const char* p){
    printf("%s", p); // %s是字符串
    exit(1); // 0正常退出，1异常退出
}
int main(){
    fd_set reads, tmps;
    FD_ZERO(&reads);
    FD_SET(0, &reads);// 监视标准输入

    timeval timeout;
    char msg[100];
    while(1) {
        tmps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int res = select(1, &tmps, 0, 0, &timeout);
        if(res == -1) {
            error_handling("select failed\n");
        } else if(res == 0) {
            printf("timeout\n");
        }  else {
            if(FD_ISSET(0, &tmps)){
                int str_len = read(0, msg, sizeof(msg));
                msg[str_len] = '0';
                printf("%d message from console %s \n", res, msg);
            }
        }
    }
}