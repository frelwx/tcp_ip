#include<cstdio>
#include<unistd.h>
int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("im child\n");
    } else {
        printf("im father, child pid is %d\n", pid);
        sleep(30);
    }
    if(pid == 0){
        printf("end child\n");
    } else {
        printf("end father\n");
    }
    return 0;
}