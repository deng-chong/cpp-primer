#include<memory>
#include<iostream>
#include<string>
#include<utility>

template<typename T>
class Vector {
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
        for (auto p = vec.beg; p != vec.end; ++p) os << *p << " ";
        return os;
    }
private:
    static std::allocator<T> alloc;
    T *beg = nullptr, *end = nullptr, *cap = nullptr;
    void reallocate(std::size_t n) {
        if (!n) {
            free();
            return;
        }
        auto new_beg = alloc.allocate(n), new_end = new_beg;
        for (auto p = beg; p != end && new_end != new_beg + n; ++p) alloc.construct(new_end++, std::move(*p));
        free();
        beg = new_beg;
        end = new_end;
        cap = beg + n;
    }
    void free() {
        if (beg) {
            while (end != beg) alloc.destroy(--end);
            alloc.deallocate(beg, cap - beg);
            beg = end = cap = nullptr;
        }
    }
public:
    Vector() = default;
    Vector(std::size_t, const T& =T());
    Vector(std::initializer_list<T>);
    Vector(const Vector<T>&);
    Vector(Vector<T>&&) noexcept;
    Vector<T>& operator=(const Vector<T>&);
    Vector<T>& operator=(Vector<T>&&) noexcept;
    ~Vector();

    bool empty() const noexcept { return beg == end; }
    std::size_t size() const noexcept { return end - beg; }
    std::size_t capacity() const noexcept { return cap - beg; }
    void resize(std::size_t n, const T& val=T()) {
        if (n <= size()) {
            end = beg + n;
        } else {
            auto sz = size();
            reallocate(n);
            for (auto i = sz; i < n; ++i) alloc.construct(end++, val);
        }
    }
    void reserve(std::size_t n) {
        if (n > capacity()) reallocate(n);
    }
    void shrink_to_fit() { reallocate(size()); }

    void assign(std::size_t , const T&);
    void assign(T* , T*);
    void assign(std::initializer_list<T>);
    void push_back(const T&);
    void push_back(T&&);
    void emplace_back(T&&);
    void pop_back();
    void insert(T*, const T&);
    void erase(T*);
    void swap(Vector<T>&);
    void clear() noexcept {
        while (end != beg) alloc.destroy(--end);
    }
    
    T& operator[](std::size_t idx) { return *(beg + idx); }
    T& at(std::size_t idx) { return *(beg + idx); }
    T& front() { return *beg; }
    T& back() { return *(end - 1); }
    T* data() { return beg; }
    T* begin() { return beg; }
    T* End() { return end; }
    const T* data() const { return beg; } // const objects call this one
};

template<typename T>
std::allocator<T> Vector<T>::alloc; // static members need definition outside the class body

template<typename T>
Vector<T>::Vector(std::size_t n, const T& val) {
    reallocate(n);
    std::uninitialized_fill_n(beg, n, val);
    end = beg + n;
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> ilist) {
    std::size_t n = ilist.size();
    reallocate(n);
    auto p = ilist.begin();
    for (end = beg; end != beg + n; ++end) alloc.construct(end, *p++);
}

template<typename T>
Vector<T>::Vector(const Vector& rhs) {
    reallocate(rhs.size());
    for (auto p = rhs.beg; p != rhs.end; ++p) alloc.construct(end++, *p);
}

template<typename T>
Vector<T>::Vector(Vector&& rhs) noexcept {
    beg = rhs.beg, end = rhs.end, cap = rhs.cap;
    rhs.beg = rhs.end = rhs.cap = nullptr; // 使rhs析构安全
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs) {
    if (this != &rhs) {
        auto tmp(rhs);
        swap(tmp);
    }
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& rhs) noexcept {
    if (this != &rhs) {
        auto tmp(std::move(rhs));
        rhs.beg = rhs.end = rhs.cap = nullptr; // 使rhs析构安全
        swap(tmp);
    }
    return *this;
}

template<typename T>
Vector<T>::~Vector() {
    free();
}

template<typename T>
void Vector<T>::assign(std::size_t n, const T& val) {
    if (size() < n) {
        free();
        beg = alloc.allocate(n);
        for (end = beg; end != beg + n; ++end) alloc.construct(end, val);
        cap = beg + n;
    } else {
        for (end = beg; end != beg + n; ++end) *end = val;
    }
}
    

template<typename T>
void Vector<T>::assign(T* first, T* last) {
    std::size_t n = 0;
    for (auto p = first; p != last; ++p) ++n;
    if (size() < n) {
        free();
        beg = alloc.allocate(n);
        for (end = beg; end != beg + n; ++end) alloc.construct(end, *first++);
        cap = beg + n;
    } else {
        for (end = beg; end != beg + n; ++end) *end = *first++;
    }
}

template<typename T>
void Vector<T>::swap(Vector<T>& rhs) {
    std::swap(beg, rhs.beg);
    std::swap(end, rhs.end);
    std::swap(cap, rhs.cap);
}

template<typename T>
void Vector<T>::assign(std::initializer_list<T> ilist) {
    std::size_t n = ilist.size();
    if (size() < n) {
        free();
        beg = alloc.allocate(n);
        auto p = ilist.begin();
        for (end = beg; end != beg + n; ++end) alloc.construct(end, *p++);
        cap = beg + n;
    } else {
        auto p = ilist.begin();
        for (end = beg; end != beg + n; ++end) *end = *p++;
    }
}

template<typename T>
void Vector<T>::push_back(const T& val) {
    if (end == cap) {
        auto new_sz = beg == end ? 1 : 2 * (end - beg);
        reallocate(new_sz);
    }
    alloc.construct(end++, val);
}

template<typename T>
void Vector<T>::push_back(T&& val) {
    if (end == cap) {
        auto new_sz = beg == end ? 1 : 2 * (end - beg);
        reallocate(new_sz);
    }
    alloc.construct(end++, std::move(val));
}

template<typename T>
void Vector<T>::emplace_back(T&& val) {
    if (end == cap) {
        auto new_sz = beg == end ? 1 : 2 * (end - beg);
        reallocate(new_sz);
    }
    alloc.construct(end++, std::move(val));
}

template<typename T>
void Vector<T>::pop_back() {
    alloc.destroy(--end);
}

template<typename T>
void Vector<T>::insert(T* p, const T& val) {
    if (end == cap) {
        auto new_sz = beg == end ? 1 : 2 * (end - beg);
        reallocate(new_sz);
    }
    alloc.construct(end++, val);
    for (auto q = end - 1; q != p; --q) std::swap(*q, *(q - 1));
}

template<typename T>
void Vector<T>::erase(T* p) {
    while (p + 1 != end) std::swap(*p, *(p + 1)), ++p;
    alloc.destroy(--end);
}


int main() {
    Vector<int> v;
    for (int i = 0; i < 10; ++i) v.push_back(i);
    v.insert(v.begin(), -1);
    v.emplace_back(10);
    v.insert(v.begin() + 5, 0);
    v.insert(v.End(), 0);
    v.erase(v.begin());
    v.erase(v.begin() + 2);
    std::cout << v;
}
