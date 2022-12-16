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
        _tuple_print(std::cout,
                     std::make_tuple(args...),
                     std::make_index_sequence<sizeof...(Args)>());
    }

   private:
    std::string sep, end;

    template <typename Tuple, std::size_t... Is>
    constexpr void _tuple_print(std::ostream& os, const Tuple& t, std::index_sequence<Is...>) const {
        ((os << std::get<Is>(t) << (Is + 1 < sizeof...(Is) ? sep : end)), ...);
    }
};

#endif
