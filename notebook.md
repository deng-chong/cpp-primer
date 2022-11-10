### 1. 类的const成员函数返回值可以是非const的引用，只要这个引用的对象不是const即可。
```cpp
class A {
public:
    int &get() const { return val;} // 编译通过，运行错误！
private:
    int val = 0;
};

class B {
public:
     B(){ p = new int(0); }
     int &get() const { return *p; } // p是const，但*p不是const，所以没有问题
     ~B(){ if (p) delete p; }
private:
     int *p = nullptr;
};
```
