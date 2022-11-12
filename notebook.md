### 1. 重载和const形参(c++ primer p208)
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

- 重载函数时**顶层**const会被忽略，f(const int)会被编译成f(int)。
- 做实参时，int变量与int&变量完全相同，const int变量和const int&变量完全相同。
- 字面值、const int变量、const int&变量均不能做f(int&)的实参。
- 字面值与f(int)和f(const int&)匹配；int变量与f(int)、f(int&)和f(const int&)匹配；const int变量和f(int)和f(const int&)匹配。
- f(int&)和f(const int&)：后者的const是底层const，不构成重复定义；int（和int&）变量优先和f(int&)匹配，若不存在，可以和f(const int&)匹配\[再啰嗦一下，由以上知const int（和const int&）变量只能与f(const int&)匹配\]。

```cpp
int f(int) { return 1; }
//int f(const int) { return 2; }
int f(int&) { return 3; }
//int f(const int&) { return 4; }

int main() {
    int i = 0;
    int &ri = i;
    const int ci = 0;
    const int& rci = ci;
    std::cout << f(0);
    return 0;
}
```
![image](https://user-images.githubusercontent.com/69671764/201475805-5c5aebfe-bcac-48c7-9051-679c83382c76.png)
其中：R表示redefinition，A表示ambiguous call，$\times$表示无匹配函数


### 2. 类的const成员函数返回值可以是非const的引用，只要它引用的对象不是const即可。
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



