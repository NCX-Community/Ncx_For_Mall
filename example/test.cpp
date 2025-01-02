// #include<iostream>

// class A {
// public:
//     A() {
//         //print();
//     }
//     virtual void print() {
//         std::cout << "I am A's virtual function!" << std::endl;
//     }
// };

// class B : public A {
// public:
//     B() {
//         print();
//     }
//     void print() {
//         std::cout << "I am B's virtual function!" << std::endl;
//     }
// };

// int main() {
//     std::cout<<"test1"<<std::endl;
//     B b;
//     std::cout<<"test2"<<std::endl;
//     A* a = new B();
//     return 0;
// }

#include <iostream>

int main() {
    int a = 1;
    int& b = a;
    std::cout<<&a<<std::endl;
    std::cout<<&b<<std::endl;
}