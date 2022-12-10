#include<iostream>
#include<string>
#include<vector>
#include<memory>
using namespace std;

template<typename T, typename D = std::default_delete<T>>
class UniquePtr {
    friend void swap(UniquePtr& lhs, UniquePtr& rhs) {
        std::swap(lhs.ptr, rhs.ptr);
	std::swap(lhs.del, rhs.del);
    }
public:
    UniquePtr(T *p = nullptr, D d = D()) : ptr(p), del(d) {}
    UniquePtr(const T& one): ptr(new T(one)) {}
    UniquePtr(const UniquePtr&) = delete; // copy constructor not permitted
    UniquePtr& operator=(const UniquePtr&) = delete; // copy assignment not permitted
    UniquePtr(UniquePtr&& one) noexcept: ptr(one.ptr), del(std::move(one.del)) { one.ptr = nullptr; } // move constructor
    UniquePtr& operator=(UniquePtr&& one) noexcept {
        if (this != &one) {
            ptr = one.ptr;
            one.ptr = nullptr;
			del = std::move(one.del);
        }
        return *this;
    }
    
    T& operator*() const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }
    T* get() const noexcept { return ptr; }
    void swap(UniquePtr& one) { std::swap(ptr, one.ptr); std::swap(del, one.del); }
    void reset(T *p = nullptr) {
        delete ptr;
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

class Deleter {
public:
    Deleter(std::ostream &s = std::cerr): os(s) {}
    template<typename T>
    void operator()(T *p) const { os << "Deleting UniquePtr" << std::endl; delete p; p = nullptr; }
private:
    std::ostream &os;
};

template<typename T, typename D = std::default_delete<T>>
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
    p1.show();
    p2.show();
}
