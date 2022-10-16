#include<cstdio>
#include<unistd.h>
#include<signal.h>
void timeout(int sig){
    if(sig == SIGALRM){
        printf("time out\n");
    }
    alarm(2);
}
void keycontrol(int sig){
    if(sig == SIGINT){
        printf("ctrl+c\n");
    }
}
int main(){
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);
    for(int i = 0; i < 3; ++i){
        printf("wait ...\n");
        sleep(100);
    }
    return 0;
}