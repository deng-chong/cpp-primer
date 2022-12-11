#include <cassert>
#include <functional>
#include <iostream>
#include <memory>

template <typename T>
class SharedPtr {
    friend void swap(SharedPtr<T> &lhs, SharedPtr<T> &rhs) {
        lhs.swap(rhs);
    }

   public:
    // default deleter: nullptr (function pointer)
    SharedPtr(T *p = nullptr, std::function<void(T *)> d = nullptr) : ptr(p), cnt(nullptr), del(d) {
        if (ptr != nullptr)
            cnt = new std::size_t(1);
    }

    explicit SharedPtr(const T &x) {  // explicit: avoid implicit conversion
        std::cout << "memory successfully allocated\n";
        ptr = new T(x);
        cnt = new std::size_t(1);
    }

    SharedPtr(const SharedPtr<T> &rhs) {
        std::cout << "copy construct\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        del = rhs.del;
        assert(!ptr && !cnt || ptr && cnt);
        if (cnt)
            ++*cnt;
        std::cout << "copy construct done\n";
    }

    SharedPtr(SharedPtr<T> &&rhs) noexcept {
        std::cout << "move construct\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        del = rhs.del;
        rhs.ptr = nullptr;
        rhs.cnt = nullptr;
        rhs.del = nullptr;
        assert(!ptr && !cnt || ptr && cnt);
        std::cout << "move construct done\n";
    }

    SharedPtr &operator=(const SharedPtr<T> &rhs) {
        std::cout << "copy assign\n";
        if (this != &rhs) {
            if (ptr != rhs.ptr) {
                destroy();
                if (rhs.cnt)
                    ++*rhs.cnt;
            }
            ptr = rhs.ptr;
            cnt = rhs.cnt;
            del = rhs.del;
        }
        std::cout << "copy assign done\n";
        return *this;
    }

    SharedPtr &operator=(SharedPtr<T> &&rhs) {
        std::cout << "move assign\n";
        if (this != &rhs) {
            if (ptr != rhs.ptr)
                destroy();
            ptr = rhs.ptr;
            cnt = rhs.cnt;
            del = rhs.del;
            rhs.ptr = nullptr;
            rhs.cnt = nullptr;
            rhs.del = nullptr;
        }
        std::cout << "move assign done\n";
        return *this;
    }

    ~SharedPtr() { destroy(); }

    void swap(SharedPtr<T> &rhs) {
        std::swap(ptr, rhs.ptr);
        std::swap(cnt, rhs.cnt);
        std::swap(del, rhs.del);
    }

    std::size_t use_count() const noexcept { return cnt ? *cnt : 0; }

    T &operator*() const {
        if (!ptr)
            throw std::runtime_error("no object!");
        return *ptr;
    }

    T *operator->() const noexcept { return ptr; }

    T *get() const noexcept { return ptr; }

    void reset(T *p = nullptr, std::function<void(T *)> d = nullptr) {
        if (ptr != p) {
            destroy();
            ptr = p;
            if (ptr)
                cnt = new std::size_t(1);
        }
        del = d;
    }

    explicit operator bool() const { return ptr != nullptr; }  // bool type conversion

   private:
    T *ptr;
    std::size_t *cnt;
    std::function<void(T *)> del;

    void destroy() {
        std::cout << "function destroy called\n";
        assert(!ptr && !cnt || ptr && cnt && *cnt);
        if (cnt && --*cnt == 0) {
            std::cout << "reference count decreased\n";
            del ? del(ptr) : delete ptr;
            delete cnt;
            std::cout << "memory successfully freed\n";
        }
        ptr = nullptr;
        cnt = nullptr;
        del = nullptr;
    }
};

//template class SharedPtr<int>;

int main() {
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
