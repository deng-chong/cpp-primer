#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>


class Printer {
   public:
    Printer(const std::string& sep = " ", const std::string& end = "\n") : __sep(sep), __end(end) {}

    void set_separator(const std::string& sep) {
        __sep = sep;
    }

    void set_end(const std::string& end) {
        __end = end;
    }

    template <typename... Args>
    void print(const Args&... args) {
        _print(args...);
    }

   private:
    std::string __sep, __end;

    template <typename T>
    void _print(const T& t) {
        std::cout << t << __end;
    }

    template <typename T, typename... Args>
    void _print(const T& t, const Args&... args) {
        std::cout << t << __sep;
        _print(args...);
    }
};

int main() {
    int a = 0;
    double b = 0.5;
    char s[] = "abc";
    Printer pr;
    pr.print(a, b, s, a, b, s, s, s, "end.");
    pr.set_separator(", ");
    pr.print(a, b, s, a, b, s, s, s, "end.");
}