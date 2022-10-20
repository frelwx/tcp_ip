#include<cstdio>
#include<unistd.h>
int main(){
    // create pipe
    int fds1[2], fds2[2];
    pipe(fds1);
    pipe(fds2);


    char str1[] = "who are you?";
    char str2[] = "thank you";
    char buf[30] = {};
    pid_t pid = fork();
    if(pid == 0) {
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, sizeof(buf));
        printf("im son %s\n", buf);
    } else {
        read(fds1[0], buf, sizeof(buf));
        printf("im parent %s\n", buf);
        write(fds2[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}
