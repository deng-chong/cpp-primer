#include<vector>
#include<string>
#include<set>
#include<unordered_set>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<functional>
#include<memory>
#include<iostream>
#include<sstream>
#include <assert.h>
using namespace std;


template <class T>
class SharedPtr {
public:
    SharedPtr() = default;
    explicit SharedPtr(T x) { // explicit: avoid implicit conversion
        cout << "val " << x << " memory successfully allocated\n";
        ptr = new T(x);
        cnt = new size_t(1);
    }
    SharedPtr(const SharedPtr<T>& one) {
        cout << "construct\n";
        if (ptr != one.ptr) {
            if (ptr) destroy();
            if (one.ptr) {
                ptr = one.ptr;
                cnt = one.cnt;
                ++*cnt;
            }
        }
        cout << "constuct done\n";
    }
    SharedPtr& operator=(const SharedPtr<T>& one) {
        cout << "assign\n";
        if (ptr != one.ptr) {
            if (ptr) destroy();
            if (one.ptr) {
                ptr = one.ptr;
                cnt = one.cnt;
                ++*cnt;
            }
        }
        cout << "assign done\n";
        return *this;
    }
    T operator*() { return *ptr; }
    T* operator->() { return ptr; }
    T* get() { return ptr; }
    ~SharedPtr() {
        destroy();
    }

private:
    T* ptr = nullptr;
    size_t* cnt = nullptr;

    void destroy() {
        assert(ptr && cnt && *cnt || !ptr && !cnt);
        cout << "function destroy called\n";
        if (!ptr) return;
        cout << "val " << *ptr << " cnt " << *cnt << " reference count decreased\n";
        --*cnt;
        if (!*cnt) {
            cout << "memory successfully freed\n";
            delete ptr;
            delete cnt;
            ptr = nullptr;
            cnt = nullptr;
        }
    }
};

int main() {
    
    SharedPtr<int> p(1);
    cout << "................" << endl;
    SharedPtr<int> q;
    cout << "................" << endl;
    q = p;
    cout << "................\n" << endl;
    
    
    q = SharedPtr<int>(2);
    cout << "................" << endl;
    cout << *q << endl;
    cout << "................" << endl; 
    p = q;
    cout << "................" << endl;
    cout << *p << endl;
    cout << "................" << endl;
    
    cout << "\nEND............." << endl;
    
}
