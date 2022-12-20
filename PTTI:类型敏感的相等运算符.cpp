#include <iostream>
#include <typeinfo>

class A {  // 必须有虚函数，如 equal
    friend bool operator==(const A&, const A&);

   protected:
    virtual bool equal(const A& rhs) const {  // 应为const函数
        std::cout << "A::equal called\n";
        return i == rhs.i;
    }

    // or private:
    int i = 0;
};

class B : public A {  // 若为其他继承，则派生类到基类不可转换
   protected:
    virtual bool equal(const A& rhs) const {  // 应为const函数
        std::cout << "B::equal called\n";
        if (!A::equal(*this)) return false;  // 不能用 rhs.equal(*this)
        auto r = dynamic_cast<const B&>(rhs);
        return j == r.j;
    }

    // or private:
    int j = 0;
};

bool operator==(const A& lhs, const A& rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.equal(rhs);
}

int main() {
    A a1, a2;
    B b1, b2;
    std::cout << (a1 == a2) << "\n\n"
              << (a1 == b2) << "\n\n"
              << (b1 == a2) << "\n\n"
              << (b1 == b2);
}
