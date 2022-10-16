#include<cstdio>
#include<iostream>
#include<arpa/inet.h>
#include<string.h>
using namespace std;
struct S { int f1, f2; }; 
int main(){
    int16_t a = 0x1234;
    int8_t b = *(int8_t*)(&a);
    cout << sizeof(struct S)  << endl;
    printf("%#x\n", a);
    printf("%#x\n", b);
    int i = 0;
    i = (i++) + (i++);
    cout << i << endl;
}

