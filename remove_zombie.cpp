#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
void read_childproc(int sig){
    printf("one child end!!!!!!!!!!!!!!!!!!!!!!!!!!%d\n", sig);
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        if(WIFEXITED(status)){
            printf("remove child id %d\n", pid);
            printf("child send %d\n", WEXITSTATUS(status));
        }
    }
}
int main(){
    // struct sigaction act;
    // act.sa_handler = read_childproc;
    // sigemptyset(&act.sa_mask);
    // act.sa_flags = 0;
    // sigaction(SIGCHLD, &act, 0);

    pid_t pid = fork();

    if(pid == 0){
        printf("im child one \n");
        sleep(5);
        printf("child two end\n");
        return 12;
    } else {
        // sleep(20);
        printf("child pid %d\n", pid);
        pid = fork();
        if(pid == 0){
            printf("im child two\n");
            sleep(5);
            printf("child two end\n");
            return 24;
        } else {
            printf("child pid %d\n", pid);
            sleep(20);
            int status;

            pid = waitpid(-1, &status, WNOHANG);
            if(WIFEXITED(status)){
                printf("#############remove child id %d\n", pid);
                printf("child send %d\n", WEXITSTATUS(status));
            }
            sleep(10);
            pid = waitpid(-1, &status, WNOHANG);
            if(WIFEXITED(status)){
                printf("#############remove child id %d\n", pid);
                printf("child send %d\n", WEXITSTATUS(status));
            }
            for(int i = 0 ; i < 5; ++i){
                printf("wait.....\n");
                sleep(5);
            }
        }
    }
    return 0;
}