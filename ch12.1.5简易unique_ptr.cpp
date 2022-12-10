#include<iostream>
#include<string>
#include<vector>
#include<memory>
using namespace std;

template<typename T, typename D = std::default_delete<T>>
class UniquePtr {
public:
    UniquePtr(T *p = nullptr, D d = D()) : ptr(p), del(d) {}
    UniquePtr(const T& one): ptr(new T(one)) {}
    UniquePtr(const UniquePtr&) = delete; // copy constructor not permitted
    UniquePtr& operator=(const UniquePtr&) = delete; // copy assignment not permitted
    UniquePtr(UniquePtr&& one) noexcept: 
        ptr(std::move(one.ptr)), del(std::move(one.del)) { one.ptr = nullptr; } // move constructor
    UniquePtr& operator=(UniquePtr&& one) noexcept {
        if (this != &one) {
            del(ptr);
            ptr = std::move(one.ptr);
	        del = std::move(one.del);
            one.ptr = nullptr;
        }
        return *this;
    }

    explicit operator bool() const { return ptr != nullptr; }
    T& operator*() const noexcept { 
        if(ptr == nullptr) throw std::runtime_error("no object!"); 
        return *ptr; 
    }
    T* operator->() const noexcept { return &this->operator*(); }
    T* get() const noexcept { return ptr; }
    void swap(UniquePtr& one) { std::swap(ptr, one.ptr); std::swap(del, one.del); }
    
    void reset(T *p = nullptr) {
        del(ptr);
        ptr = p;
    }
    T* release() {
        auto p = ptr;
        ptr = nullptr;
        return p;
    }
    void show() const { if (ptr) for (auto x:*ptr) cout << x << " "; cout << endl; }
    ~UniquePtr() { del(ptr); ptr = nullptr;}
private:
    T *ptr;
    D del;
};

template<typename T, typename D>
void swap(UniquePtr<T, D>& lhs, UniquePtr<T, D>& rhs) { lhs.swap(rhs); }

class Deleter {
public:
    template<typename T>
    void operator()(T *p) const { 
        std::cout << "Deleting UniquePtr" << std::endl; 
        delete p; 
        p = nullptr; 
    }
};

template<typename T, typename D>
UniquePtr<T, D> f() {
    return UniquePtr<T, D>();
}

int main() {
    vector<string> v1{"A", "B", "C"}, v2{"Book", "See", "Hi"};
    UniquePtr<vector<string>, Deleter> p1(v1), p2(v2);
    UniquePtr<vector<string>, Deleter> p(f<vector<string>, Deleter>());
    p.reset(p1.release());
    p.show();
    p1.reset(p2.release());
    p2.reset(p.release());
    swap(p1, p2);
    p1.show();
    p2.show();
}
