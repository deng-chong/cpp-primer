#ifndef PRINTER_H
#define PRINTER_H

#include "shift_left_overloads.h"

class Printer {
   public:
    Printer(const std::string& _sep = ", ", const std::string& _end = "\n") : sep(_sep), end(_end) {}

    Printer& set_separator(const std::string& _sep) {
        sep = _sep;
        return *this;
    }

    Printer& set_end(const std::string& _end) {
        end = _end;
        return *this;
    }

    template <typename... Args>
    void print(const Args&... args) const {
        _print(args...);
    }

   private:
    std::string sep, end;

    void _print() const {  // 必须有，否则下面的_print在实参包大小为零时没有可调用函数，编译错误
        std::cout << end;
    }

    template <typename T, typename... Args>
    void _print(const T& t, const Args&... args) const {
        auto n = sizeof...(args);
        if (!n) {
            std::cout << t << end;
        } else {
            std::cout << t << sep;
            _print(args...);
        }
    }
};

#endif
