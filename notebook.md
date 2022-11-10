### 1. 类的const成员函数返回值可以是非const的引用，只要它引用的对象不是const即可。
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

### 2. 重载和const形参(c++ primer p208)
拥有顶层const的形参无法和另一个没有顶层const的形参区分开来
```cpp
int f(int);
int f(const int); // 重复声明 int f(int)

int f(int*);
int f(int* const); // 重复声明 int f(int*)
```
如果形参是指针或引用，则通过区分其指向的是常量对象还是非常量对象（而不管其本身是不是const），可以实现函数重载，此时const是底层的
```cpp
int f(int&);       // 函数作用于int变量的引用
int f(const int&); // 新函数，作用于常量引用

int f(int*);             // 新函数，作用于int变量的指针
int f(const int*);       // 新函数，作用于const int变量的指针
int f(int* const);       // 重复声明 int f(int*)
int f(const int* const); // 重复声明 int f(const int*)
```
- 在 `(1)int (2)int& (3)const int (4)const int&` 中，只有`(2)`和`(4)`可以进行函数重载！（`int`换成其他类型也一样。）
推断：对象和它的引用是区分不开、不能重载的！
- 在`(1)int* (2)const int* (3)int* const (4)const int* const` 中，只有`(1)`与`(3)`、`(2)`与`(4)`**不能重载**，因为这两对都仅是顶层const不一样。
