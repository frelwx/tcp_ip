#include<cstdio>
#include<iostream>
#include<arpa/inet.h>
using namespace std;
int main(){
    sockaddr_in x, y;
    char *p;
    x.sin_addr.s_addr = htonl(0x10203040);
    y.sin_addr.s_addr = htonl(0x10101010);
    p = inet_ntoa(x.sin_addr);
    cout << p << endl;
    inet_ntoa(y.sin_addr);
    cout << p << endl;
    cout << *p << endl;
}

