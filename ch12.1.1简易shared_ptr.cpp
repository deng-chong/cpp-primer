#include<assert.h>
#include<iostream>


template <class T>
class SharedPtr {
    friend std::ostream& operator<<(std::ostream& os, const SharedPtr<T>& sp) {
        os << sp.ptr;
        return os;
    }
    friend void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) {
        std::swap(lhs.ptr, rhs.ptr);
        std::swap(lhs.cnt, rhs.cnt);
    }
public:
    SharedPtr() = default;

    explicit SharedPtr(T x) { // explicit: avoid implicit conversion
        std::cout // << "val " << x << " "
            << "memory successfully allocated\n";
        ptr = new T(x);
        cnt = new std::size_t(1);
    }

    SharedPtr(const SharedPtr<T>& rhs) {
        std::cout << "copy construct\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        if (cnt) ++*cnt;
        std::cout << "copy constuct done\n";
    }

    SharedPtr(SharedPtr<T>&& rhs) noexcept {
        std::cout << "move construct\n";
        ptr = rhs.ptr;
        cnt = rhs.cnt;
        rhs.ptr = nullptr;
        rhs.cnt = nullptr;
        std::cout << "move constuct done\n";
    }

    SharedPtr& operator=(const SharedPtr<T>& rhs) {
        std::cout << "copy assign\n";
        if (this != &rhs) {
            destroy();
            ptr = rhs.ptr;
            cnt = rhs.cnt;
            if (cnt) ++*cnt;
        }
        std::cout << "copy assign done\n";
        return *this;
    }

    SharedPtr& operator=(SharedPtr<T>&& rhs) {
        std::cout << "move assign\n";
        if (this != &rhs) {
            destroy();
            ptr = rhs.ptr;
            cnt = rhs.cnt;
            rhs.ptr = nullptr;
            rhs.cnt = nullptr;
        }
        std::cout << "move assign done\n";
        return *this;
    }

    ~SharedPtr() { destroy(); }

    void swap(SharedPtr<T>& rhs) {
        std::swap(ptr, rhs.ptr);
        std::swap(cnt, rhs.cnt);
    }

    T& operator*() const noexcept { return *ptr; }

    T* operator->() const noexcept { return ptr; }

    T* get() const noexcept { return ptr; }

    void reset(T *p = nullptr) {
        if (ptr != p) {
            destroy();
            ptr = p;
            if (ptr) cnt = new std::size_t(1);
        }
    }
    
    explicit operator bool() const { return ptr != nullptr; } // bool type conversion

private:
    T *ptr = nullptr;
    std::size_t *cnt = nullptr;

    void destroy() {
        assert(ptr && cnt && *cnt || !ptr && !cnt);
        std::cout << "function destroy called\n";
        if (!ptr) return;
        if (--*cnt == 0) {
            std::cout // << "val " << *ptr << " cnt " << *cnt << " "
                << "reference count decreased\n";
            delete ptr;
            delete cnt;
            std::cout << "memory successfully freed\n";
        }
        ptr = nullptr;
        cnt = nullptr;
    }
};

int main() {
    
    SharedPtr<int> p(1);
    std::cout << "................" << std::endl;
    SharedPtr<int> q;
    std::cout << "................" << std::endl;
    q = p;
    std::cout << "................\n" << std::endl;
    
    
    q = SharedPtr<int>(2);
    std::cout << "................" << std::endl;
    std::cout << *q << std::endl;
    std::cout << "................" << std::endl; 
    p = q;
    std::cout << "................" << std::endl;
    std::cout << *p << std::endl;
    std::cout << "................" << std::endl;
    
    std::cout << "\nEND............." << std::endl;
    
    //if (p) std::cout << "**************************";
    
}
