#include<sys/wait.h>
#include<cstdio>
#include<unistd.h>
int main(){
    int status;
    pid_t pid = fork();

    if(pid == 0){
        sleep(15);
        return 24;
    } else {
        while(!waitpid(-1, &status, WNOHANG)){
            sleep(3);
            printf("sleep 3s\n");
        }
        if(WIFEXITED(status)){
            printf("child send :%d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}