//三种方法为tuple重载<<
//方法1

template <typename Tuple, std::size_t N>
struct TuplePrinter {
    static std::ostream& print(std::ostream& os, const Tuple& t) {
        return TuplePrinter<Tuple, N - 1>::print(os, t) << ", " << std::get<N - 1>(t);
    }
};

template <typename Tuple>
struct TuplePrinter<Tuple, 1> {
    static std::ostream& print(std::ostream& os, const Tuple& t) {
        return os << std::get<0>(t);
    }
};

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    constexpr std::size_t N = sizeof...(Args);
    if (N) TuplePrinter<decltype(t), N>::print(os, t);
    os << ")";
    return os;
}

// 方法2

template <std::size_t I, typename... Args>
typename std::enable_if<(I + 1 == sizeof...(Args)), std::ostream&>::type
_tuple_print(std::ostream& os, const std::tuple<Args...>& t) {
    return os << std::get<I>(t);
}

template <std::size_t I, typename... Args>
typename std::enable_if<(I + 1 < sizeof...(Args)), std::ostream&>::type
_tuple_print(std::ostream& os, const std::tuple<Args...>& t) {
    os << std::get<I>(t) << ", ";
    return _tuple_print<I + 1, Args...>(os, t);
}

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    if (sizeof...(Args)) _tuple_print<0, Args...>(os, t);
    os << ")";
    return os;
}

// 方法3

template <std::size_t I, typename... Args>
std::ostream& _tuple_print(std::ostream& os, const std::tuple<Args...>& t) {
    os << std::get<I>(t) << (I + 1 < sizeof...(Args) ? ", " : "");
    if constexpr (I + 1 < sizeof...(Args))  // if constexpr: since c++17
        _tuple_print<I + 1, Args...>(os, t);
    return os;
}

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    if (sizeof...(Args)) _tuple_print<0, Args...>(os, t);
    os << ")";
    return os;
}
