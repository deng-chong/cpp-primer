#include <iostream>
#include <memory>
using namespace std;

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

template <typename T>
class SharedPtr {
    template <typename U>
    friend class SharedPtr;

   public:
    template <typename U = T>
    SharedPtr(U *p = nullptr) : ptr(p), cnt(nullptr) {
        if (ptr)
            cnt = new std::size_t(1);
    }

    template <typename U = T>
    explicit SharedPtr(const U &x) {  // explicit: avoid implicit conversion
        std::cout << "memory successfully allocated\n";
        ptr = new U(x);
        cnt = new std::size_t(1);
    }

    SharedPtr(const SharedPtr &rhs) noexcept {
        std::cout << "copy construct T\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        assert(!ptr && !cnt || ptr && cnt);
        if (cnt)
            ++*cnt;
        std::cout << "copy construct T done\n";
    }

    template <typename U>
    SharedPtr(const SharedPtr<U> &rhs) noexcept {
        std::cout << "copy construct U\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        assert(!ptr && !cnt || ptr && cnt);
        if (cnt)
            ++*cnt;
        std::cout << "copy construct U done\n";
    }

    SharedPtr(SharedPtr &&rhs) noexcept {
        std::cout << "move construct T\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        rhs.ptr = nullptr;
        rhs.cnt = nullptr;
        assert(!ptr && !cnt || ptr && cnt);
        std::cout << "move construct T done\n";
    }

    template <typename U>
    SharedPtr(SharedPtr<U> &&rhs) noexcept {
        std::cout << "move construct U\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        rhs.ptr = nullptr;
        rhs.cnt = nullptr;
        assert(!ptr && !cnt || ptr && cnt);
        std::cout << "move construct U done\n";
    }

    SharedPtr &operator=(const SharedPtr &rhs) noexcept {
        std::cout << "copy assign T\n";
        if (this != &rhs) {
            if (ptr != rhs.ptr) {
                destroy();
                if (rhs.cnt)
                    ++*rhs.cnt;
            }
            ptr = rhs.ptr;
            cnt = rhs.cnt;
        }
        std::cout << "copy assign T done\n";
        return *this;
    }

    template <typename U>
    SharedPtr &operator=(const SharedPtr<U> &rhs) noexcept {
        std::cout << "copy assign U\n";
        if (this != &rhs) {
            if (ptr != rhs.ptr) {
                destroy();
                if (rhs.cnt)
                    ++*rhs.cnt;
            }
            ptr = rhs.ptr;
            cnt = rhs.cnt;
        }
        std::cout << "copy assign U done\n";
        return *this;
    }

    SharedPtr &operator=(SharedPtr &&rhs) noexcept {
        std::cout << "move assign T\n";
        if (this != &rhs) {
            if (ptr != rhs.ptr)
                destroy();
            ptr = rhs.ptr;
            cnt = rhs.cnt;
            rhs.ptr = nullptr;
            rhs.cnt = nullptr;
        }
        std::cout << "move assign T done\n";
        return *this;
    }

    template <typename U>
    SharedPtr &operator=(SharedPtr<U> &&rhs) noexcept {
        std::cout << "move assign U\n";
        if (this != &rhs) {
            if (ptr != rhs.ptr)
                destroy();
            ptr = rhs.ptr;
            cnt = rhs.cnt;
            rhs.ptr = nullptr;
            rhs.cnt = nullptr;
        }
        std::cout << "move assign U done\n";
        return *this;
    }

    ~SharedPtr() { destroy(); }

    void swap(SharedPtr &rhs) {
        std::swap(ptr, rhs.ptr);
        std::swap(cnt, rhs.cnt);
    }

    std::size_t use_count() const noexcept { return cnt ? *cnt : 0; }

    T &operator*() const {
        if (!ptr)
            throw std::runtime_error("no object!");
        return *ptr;
    }

    T *operator->() const noexcept { return ptr; }

    T *get() const noexcept { return ptr; }

    template <typename U = T>
    void reset(U *p = nullptr) {
        if (ptr != p) {
            destroy();
            ptr = p;
            if (ptr)
                cnt = new std::size_t(1);
        }
    }

    explicit operator bool() const { return ptr != nullptr; }  // bool type conversion

   private:
    T *ptr;
    std::size_t *cnt;

    void destroy() {
        std::cout << "function destroy called\n";
        assert(!ptr && !cnt || ptr && cnt && *cnt);
        if (cnt && --*cnt == 0) {
            std::cout << "reference count decreased\n";
            delete ptr;
            delete cnt;
            std::cout << "memory successfully freed\n";
        }
        ptr = nullptr;
        cnt = nullptr;
    }
};

template <typename T, typename... Args>
SharedPtr<T> makeShared(Args &&...args) {
    return new T(std::forward<Args>(args)...);
}

template <typename T>
void swap(SharedPtr<T> &lhs, SharedPtr<T> &rhs) {
    lhs.swap(rhs);
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

void fun(const SharedPtr<A> &) {  // 形参也可以是 SharedPtr<A>，但不能是 SharedPtr<A>&
    std::cout << "Get in\n";
}

int main() {
    auto pointee = makeShared<B>();
    std::cout << "Before\n";
    fun(pointee);
    std::cout << "End\n";

    auto ptr = makeShared<std::string>(10, 'a');
    std::cout << ptr.use_count() << " " << *ptr << std::endl;

    A *pa = new A();
    B *pb = new B();
    SharedPtr<A> spa(pa), spb(pb);
    spa = spb;
    spa->show();
    spb->show();

    SharedPtr<int> p(1);
    std::cout << "................" << std::endl;
    SharedPtr<int> q;
    std::cout << "................" << std::endl;
    q = p;
    std::cout << p.use_count() << " " << *p << std::endl;
    std::cout << q.use_count() << " " << *q << std::endl;
    std::cout << "................\n\n";

    q = SharedPtr<int>(20);
    std::cout << "................" << std::endl;
    std::cout << p.use_count() << " " << *p << std::endl;
    std::cout << q.use_count() << " " << *q << std::endl;
    std::cout << "................" << std::endl;
    p = q;
    std::cout << "................" << std::endl;
    std::cout << *p << std::endl;
    std::cout << "................" << std::endl;

    if (p)
        std::cout << "**************************\n";
    std::cout << "\nEND............." << std::endl;
}
