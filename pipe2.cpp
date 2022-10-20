#include<cstdio>
#include<unistd.h>
int main(){
    // create pipe
    int fds[2];
    pipe(fds);
    printf("%d %d\n", fds[0], fds[1]);


    char str1[] = "who are you?";
    char str2[] = "thank you";
    char buf[30] = {};
    pid_t pid = fork();
    if(pid == 0) {
        write(fds[1], str1, sizeof(str1));
        // sleep(2);
        read(fds[0], buf, sizeof(buf));
        printf("im son %s\n", buf);
    } else {
        read(fds[0], buf, sizeof(buf));
        printf("im parent %s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}
