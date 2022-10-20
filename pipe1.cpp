#include<cstdio>
#include<unistd.h>
int main(){
    // create pipe
    int fds[2];
    printf("%d %d\n", fds[0], fds[1]);
    pipe(fds);
    printf("%d %d\n", fds[0], fds[1]);


    char str[] = "who are you?";
    char buf[30] = {};
    pid_t pid = fork();
    if(pid == 0) {
        write(fds[1], str, sizeof(str));
    } else {
        read(fds[0], buf, sizeof(buf));
        printf("%s\n", buf);
    }
    return 0;
}
