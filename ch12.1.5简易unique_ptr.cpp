#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

template <typename T, typename D = std::default_delete<T>>
class UniquePtr {
   public:
    template <typename U = T>
    UniquePtr(U* p = nullptr, D d = D()) : ptr(p), del(d) {}

    template <typename U = T>
    UniquePtr(const U& one) : ptr(new U(one)) {}

    UniquePtr(const UniquePtr&) = delete;             // copy constructor not permitted
    UniquePtr& operator=(const UniquePtr&) = delete;  // copy assignment not permitted

    UniquePtr(UniquePtr&& one) noexcept : ptr(std::move(one.ptr)), del(std::move(one.del)) { one.ptr = nullptr; }  // move constructor
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
    T& operator*() const {
        if (!ptr) throw std::runtime_error("no object!");
        return *ptr;
    }
    T* operator->() const noexcept { return &this->operator*(); }
    T* get() const noexcept { return ptr; }
    void swap(UniquePtr& one) {
        std::swap(ptr, one.ptr);
        std::swap(del, one.del);
    }

    template <typename U = T>
    void reset(U* p = nullptr) {
        if (p == ptr) return;
        del(ptr);
        ptr = p;
    }
    T* release() {
        auto p = ptr;
        ptr = nullptr;
        return p;
    }

    ~UniquePtr() {
        del(ptr);
        ptr = nullptr;
    }

    void show() const {
        if (ptr) {
            for (auto& x : *ptr)
                std::cout << x << " ";
            std::cout << std::endl;
        }
    }

   private:
    T* ptr;
    D del;
};

template <typename T, typename D>
void swap(UniquePtr<T, D>& lhs, UniquePtr<T, D>& rhs) { lhs.swap(rhs); }

template <typename T>
class Deleter {
   public:
    void operator()(T* p) const {
        std::cout << "Deleting UniquePtr" << std::endl;
        delete p;
        p = nullptr;
    }
};

template <typename T, typename D>
UniquePtr<T, D> f() {
    return UniquePtr<T, D>();
}

class A {
   public:
    int a = 0;
    virtual ~A() = default;
    virtual void show() const {
        std::cout << "A: " << a << "\n";
    }
};

class B : public A {
   public:
    std::string s = "B";
    virtual void show() const override {
        std::cout << "B: " << a << " " << s << " "
                  << "\n";
    }
};

int main() {
    A* pa = new A();
    B* pb = new B();
    UniquePtr<A> spa(pa), spb(pb);
    spa->show();
    spb->show();
    spa.reset(spb.release());
    spa->show();
    //spb->show();

    vector<string> v1{"A", "B", "C"}, v2{"Book", "See", "Hi"};
    UniquePtr<vector<string>, Deleter<vector<string>>> p1(v1), p2(v2);
    UniquePtr<vector<string>, Deleter<vector<string>>> p(f<vector<string>, Deleter<vector<string>>>());
    p.reset(p1.release());
    p.show();
    p1.reset(p2.release());
    p2.reset(p.release());
    swap(p1, p2);
    p1.show();
    p2.show();
}
