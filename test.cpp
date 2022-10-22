#include<bits/stdc++.h>
using namespace std;
class singleton {
private:
   singleton() {}; // 构造函数私有，无法通过getinstance之外的方法构造对象
   singleton(const singleton& other)=delete; //禁用拷贝构造
public:
   static singleton& get_instance() {
      static singleton tmp;
      return tmp;
   }
};
// 可以
// class A{};
// static A tmp;
// 不可以
class A;
static A tmp;
int main(){
   singleton &z = singleton::get_instance();
}