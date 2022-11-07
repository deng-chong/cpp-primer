#include<bits/stdc++.h>
using namespace std;

typedef int Type;
Type initVal() {return Type(-1000); } // it is never called, haha

class A {
public:
    typedef double Type;
    Type setVal(Type); // Type -> double
    Type initVal() {return Type(3.14);} // Type -> double
public:
    int val = 0;
};

A::Type A::setVal(Type parm) { // double A::setVal(double parm)
    val = parm + initVal();
    return val;
}

/*
Type A::setVal(Type parm) {// some code} // Error: 'int A::setVal(double parm)' does not match any in class 'A'
A::Type A::setVal(::Type parm) {// some code} // Error: 'double A::setVal(int parm)' does not match any in class 'A'
A::Type A::setVal(Type parm) {// some code} // Error: 'double A::setVal(double parm)' does not match any in class 'A'
*/

/**********************************************************************************************************************/

class B { // the difference between class A and B is the place of 'typedef double Type;'
public:
    Type setVal(Type); // Type -> int
    Type initVal() {return Type(3.14);} // Type -> int
public:
    typedef double Type;
    int val = 0;
};

Type B::setVal(::Type parm) { // int B::setVal(int parm)
    val = parm + initVal();
    return val;
}

/*
Type B::setVal(Type parm) {// some code} // Error: 'int B::setVal(double parm)' does not match any in class 'B'
B::Type B::setVal(::Type parm) {// some code} // Error: 'double B::setVal(int parm)' does not match any in class 'B'
B::Type B::setVal(Type parm) {// some code} // Error: 'double B::setVal(double parm)' does not match any in class 'B'
*/


int main() {
    A a;
    a.setVal(0.86);
    cout << a.val << endl; // result: 4
    B b;
    b.setVal(0.86);
    cout << b.val << endl; // result: 3
    return 0;
}
