#include<iostream>
#include<string>
#include<vector>
#include<memory>
using namespace std;

template<class T>
class UniquePtr {
    friend void swap(UniquePtr& lhs, UniquePtr& rhs) { std::swap(lhs.ptr, rhs.ptr); }
public:
    UniquePtr() = default; // default constructor
    explicit UniquePtr(T* p): ptr(p) {}
    explicit UniquePtr(const T& one): ptr(new T(one)) {}
    UniquePtr(const UniquePtr&) = delete; // copy constructor not permitted
    UniquePtr& operator=(const UniquePtr&) = delete; // copy assignment not permitted
    UniquePtr(UniquePtr&& one): ptr(one.ptr) { one.ptr = nullptr; } // move constructor
    UniquePtr& operator=(UniquePtr&& one) { ptr = one.ptr, one.ptr = nullptr; }
    
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    T* get() { return ptr; }
    void swap(UniquePtr& one) { std::swap(ptr, one.ptr); }
    void reset(T* p = nullptr) {
        if (ptr) delete ptr;
        ptr = p;
    }
    T* release() {
        auto p = ptr;
        ptr = nullptr;
        return p;
    }
    void show() const { if (ptr) for (auto x:*ptr) cout << x << " "; cout << endl; }
    ~UniquePtr() { if (ptr) delete ptr; }
private:
    T *ptr;
};

template<class T>
UniquePtr<T> f() { 
    return UniquePtr<T>(); 
}


int main() {
    vector<string> v1{"A", "B", "C"}, v2{"Book", "See", "Hi"};
    UniquePtr<vector<string>> p1(v1), p2(v2);
    UniquePtr<vector<string>> p(f<vector<string>>());
    p.reset(p1.release());
    p.show();
    p1.reset(p2.release());
    p1.show();
    p2.show();
}