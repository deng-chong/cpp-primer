#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>

template <typename T>
class Vector {
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
        os << "[";
        auto p = vec._beg;
        if (p != vec._end) {
            os << *p;
            for (++p; p != vec._end; ++p) os << ", " << *p;
        }
        os << "]";
        return os;
    }

   private:
    static std::allocator<T> alloc;
    T *_beg = nullptr, *_end = nullptr, *_cap = nullptr;
    void reallocate(std::size_t n) {
        if (!n) {
            free();
            return;
        }
        auto new_beg = alloc.allocate(n), new_end = new_beg;
        for (auto p = _beg; p != _end && new_end != new_beg + n; ++p) alloc.construct(new_end++, std::move(*p));
        free();
        _beg = new_beg;
        _end = new_end;
        _cap = _beg + n;
    }
    void free() {
        if (_beg) {
            while (_end != _beg) alloc.destroy(--_end);
            alloc.deallocate(_beg, _cap - _beg);
            _beg = _end = _cap = nullptr;
        }
    }

   public:
    Vector() = default;
    Vector(std::size_t, const T& = T());
    Vector(std::initializer_list<T>);
    Vector(const Vector<T>&);
    Vector(Vector<T>&&) noexcept;
    Vector<T>& operator=(const Vector<T>&);
    Vector<T>& operator=(Vector<T>&&) noexcept;
    ~Vector();

    bool empty() const noexcept { return _beg == _end; }
    std::size_t size() const noexcept { return _end - _beg; }
    std::size_t capacity() const noexcept { return _cap - _beg; }
    void resize(std::size_t n, const T& val = T()) {
        if (n <= size()) {
            _end = _beg + n;
        } else {
            auto sz = size();
            reallocate(n);
            for (auto i = sz; i < n; ++i) alloc.construct(_end++, val);
        }
    }
    void reserve(std::size_t n) {
        if (n > capacity()) reallocate(n);
    }
    void shrink_to_fit() { reallocate(size()); }

    void assign(std::size_t, const T&);
    template <typename It>
    void assign(It, It);
    void assign(std::initializer_list<T>);
    void push_back(const T&);
    void push_back(T&&);
    template <typename... Args>
    void emplace_back(Args&&...);
    void pop_back();
    void insert(T*, const T&);
    void erase(T*);
    void swap(Vector<T>&);
    void clear() noexcept {
        while (_end != _beg) alloc.destroy(--_end);
    }

    T& operator[](std::size_t idx) { return *(_beg + idx); }
    T& at(std::size_t idx) { return *(_beg + idx); }
    T& front() { return *_beg; }
    T& back() { return *(_end - 1); }
    const T& operator[](std::size_t idx) const { return *(_beg + idx); }
    const T& at(std::size_t idx) const { return *(_beg + idx); }
    const T& front() const { return *_beg; }
    const T& back() const { return *(_end - 1); }
    T* data() noexcept { return _beg; }
    T* begin() noexcept { return _beg; }
    T* end() noexcept { return _end; }
    const T* data() const noexcept { return _beg; }
    const T* begin() const noexcept { return _beg; }
    const T* end() const noexcept { return _end; }
    
    Vector<T>& sort(std::function<bool(const T&, const T&)> f = nullptr) {
        f ? std::sort(_beg, _end, f) : std::sort(_beg, _end);
        return *this;
    }
    Vector<T> sorted(std::function<bool(const T&, const T&)> f = nullptr) && {
        return sort(f);
    }
    Vector<T> sorted(std::function<bool(const T&, const T&)> f = nullptr) const & {
        return Vector<T>(*this).sorted(f);
    }
};

template <typename T>
std::allocator<T> Vector<T>::alloc;  // static members need definition outside the class body

template <typename T>
Vector<T>::Vector(std::size_t n, const T& val) {
    reallocate(n);
    std::uninitialized_fill_n(_beg, n, val);
    _end = _beg + n;
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> ilist) {
    reallocate(ilist.size());
    for (auto p = ilist.begin(); p != ilist.end(); ++p)
        alloc.construct(_end++, *p);
}

template <typename T>
Vector<T>::Vector(const Vector& rhs) {
    reallocate(rhs.size());
    for (auto p = rhs._beg; p != rhs._end; ++p)
        alloc.construct(_end++, *p);
}

template <typename T>
Vector<T>::Vector(Vector&& rhs) noexcept {
    _beg = rhs._beg, _end = rhs._end, _cap = rhs._cap;
    rhs._beg = rhs._end = rhs._cap = nullptr;  // 使rhs析构安全
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs) {
    if (this != &rhs) {
        auto tmp(rhs);
        swap(tmp);
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& rhs) noexcept {
    if (this != &rhs) {
        auto tmp(std::move(rhs));
        rhs._beg = rhs._end = rhs._cap = nullptr;  // 使rhs析构安全
        swap(tmp);
    }
    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    free();
}

template <typename T>
void Vector<T>::assign(std::size_t n, const T& val) {
    if (size() < n) {
        free();
        _beg = alloc.allocate(n);
        for (_end = _beg; _end != _beg + n; ++_end) alloc.construct(_end, val);
        _cap = _beg + n;
    } else {
        for (_end = _beg; _end != _beg + n; ++_end) *_end = val;
    }
}

template <typename T>
template <typename It>
void Vector<T>::assign(It first, It last) {
    std::size_t n = 0;
    for (auto p = first; p != last; ++p) ++n;
    if (size() < n) {
        free();
        _beg = alloc.allocate(n);
        for (_end = _beg; _end != _beg + n; ++_end) alloc.construct(_end, *first++);
        _cap = _beg + n;
    } else {
        for (_end = _beg; _end != _beg + n; ++_end) *_end = *first++;
    }
}

template <typename T>
void Vector<T>::swap(Vector<T>& rhs) {
    std::swap(_beg, rhs._beg);
    std::swap(_end, rhs._end);
    std::swap(_cap, rhs._cap);
}

template <typename T>
void Vector<T>::assign(std::initializer_list<T> ilist) {
    std::size_t n = ilist.size();
    if (size() < n) {
        free();
        _beg = alloc.allocate(n);
        auto p = ilist.begin();
        for (_end = _beg; _end != _beg + n; ++_end) alloc.construct(_end, *p++);
        _cap = _beg + n;
    } else {
        auto p = ilist.begin();
        for (_end = _beg; _end != _beg + n; ++_end) *_end = *p++;
    }
}

template <typename T>
void Vector<T>::push_back(const T& val) {
    if (_end == _cap) {
        auto new_sz = _beg == _end ? 1 : 2 * (_end - _beg);
        reallocate(new_sz);
    }
    alloc.construct(_end++, val);
}

template <typename T>
void Vector<T>::push_back(T&& val) {
    if (_end == _cap) {
        auto new_sz = _beg == _end ? 1 : 2 * (_end - _beg);
        reallocate(new_sz);
    }
    alloc.construct(_end++, std::move(val));
}

template <typename T>
template <typename... Args>
void Vector<T>::emplace_back(Args&&... args) {
    if (_end == _cap) {
        auto new_sz = _beg == _end ? 1 : 2 * (_end - _beg);
        reallocate(new_sz);
    }
    alloc.construct(_end++, std::forward<Args>(args)...);
}

template <typename T>
void Vector<T>::pop_back() {
    alloc.destroy(--_end);
}

template <typename T>
void Vector<T>::insert(T* p, const T& val) {
    if (_end == _cap) {
        auto new_sz = _beg == _end ? 1 : 2 * (_end - _beg);
        reallocate(new_sz);
    }
    alloc.construct(_end++, val);
    for (auto q = _end - 1; q != p; --q) std::swap(*q, *(q - 1));
}

template <typename T>
void Vector<T>::erase(T* p) {
    while (p + 1 != _end) std::swap(*p, *(p + 1)), ++p;
    alloc.destroy(--_end);
}

template<typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    for (std::size_t idx = 0; idx < lhs.size(); ++idx) {
        if (lhs[idx] != rhs[idx]) return false;
    }
    return true;
}

template<typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    for (std::size_t idx = 0; idx < lhs.size() && idx < rhs.size(); ++idx) {
        if (lhs[idx] < rhs[idx]) return true;
        else if (lhs[idx] > rhs[idx]) return false;
    }
    return lhs.size() < rhs.size();
}

template<typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    for (std::size_t idx = 0; idx < lhs.size() && idx < rhs.size(); ++idx) {
        if (lhs[idx] < rhs[idx]) return true;
        else if (lhs[idx] > rhs[idx]) return false;
    }
    return lhs.size() <= rhs.size();
}

template<typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs < lhs;
}

template<typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs <= lhs;
}

/*
template class Vector<int>;

int main() {
    Vector<std::string> sv;
    for (int i = 0; i < 5; ++i) sv.emplace_back(10, 'a' + i);
    std::cout << sv << std::endl;

    Vector<int> v;
    for (int i = 0; i < 10; ++i) v.push_back(i);
    v.insert(v.begin(), -1);
    v.emplace_back(10);
    v.insert(v.begin() + 5, 0);
    v.insert(v.end(), 0);
    v.erase(v.begin());
    v.erase(v.begin() + 2);
    std::cout << v << "\n";

    std::list<int> lst{111, 2, 3, 99};
    v.assign(lst.begin(), lst.end());
    std::cout << v << "\n";

    Vector<Vector<int>> vv{{100, 2}, {2, 13}, {3, 4}, {11, 2}, {11, 13}, {3, 4}};
    std::cout << vv.sorted();
}
*/
