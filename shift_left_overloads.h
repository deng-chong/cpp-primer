#ifndef SHIFT_LEFT_OVERLOADS_H
#define SHIFT_LEFT_OVERLOADS_H

/**
 *  This file defines overloads of left shift operator (<<) for various containers
 *  Author: Deng Chong
 *  Date: 2022/12/14
 */

// *******************************************
// Declarations of function templates
// *******************************************

template <typename T, typename U>
std::ostream& operator<<(std::ostream&, const std::pair<T, U>&);

template <typename... Args>
std::ostream& operator<<(std::ostream&, const std::tuple<Args...>&);

template <typename T>
std::ostream& operator<<(std::ostream&, const std::vector<T>&);

template <typename T>
std::ostream& operator<<(std::ostream&, const std::list<T>&);

template <typename T>
std::ostream& operator<<(std::ostream&, std::stack<T>);

template <typename T>
std::ostream& operator<<(std::ostream&, std::queue<T>);

template <typename T>
std::ostream& operator<<(std::ostream&, std::priority_queue<T>);

template <typename T>
std::ostream& operator<<(std::ostream&, const std::set<T>&);

template <typename T>
std::ostream& operator<<(std::ostream&, const std::multiset<T>&);

template <typename T>
std::ostream& operator<<(std::ostream&, const std::unordered_set<T>&);

template <typename T>
std::ostream& operator<<(std::ostream&, const std::unordered_multiset<T>&);

template <typename T, typename U>
std::ostream& operator<<(std::ostream&, const std::map<T, U>&);

template <typename T, typename U>
std::ostream& operator<<(std::ostream&, const std::multimap<T, U>&);

template <typename T, typename U>
std::ostream& operator<<(std::ostream&, const std::unordered_map<T, U>&);

template <typename T, typename U>
std::ostream& operator<<(std::ostream&, const std::unordered_multimap<T, U>&);

// *******************************************
// Definitions of function templates
// *******************************************

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

template <typename Tuple, std::size_t... Is>
constexpr void _tuple_print(std::ostream& os, const Tuple &t, std::index_sequence<Is...>) {
    ((os << std::get<Is>(t) << (Is + 1 < sizeof...(Is) ? ", " : "")), ...);
}

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    _tuple_print(os, t, std::make_index_sequence<sizeof...(Args)>());
    os << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    if (vec.size()) {
        os << vec[0];
        for (auto p = ++vec.begin(); p != vec.end(); ++p)
            os << ", " << *p;
    }
    os << "]";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& lst) {
    os << "[";
    if (lst.size()) {
        os << *lst.begin();
        for (auto p = ++lst.begin(); p != lst.end(); ++p)
            os << ", " << *p;
    }
    os << "]";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::stack<T> s) {
    std::vector<T> v;
    while (s.size()) v.emplace_back(s.top()), s.pop();
    os << "|";
    if (v.size()) {
        os << v.back();
        v.pop_back();
        while (v.size()) {
            os << " ~> " << v.back();
            v.pop_back();
        }
    }
    os << ">";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::queue<T> q) {
    os << "<";
    if (q.size()) {
        os << q.front();
        q.pop();
        while (q.size()) {
            os << " <~ " << q.front();
            q.pop();
        }
    }
    os << "<";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::priority_queue<T> q) {
    os << "[";
    if (q.size()) {
        os << q.top();
        q.pop();
        while (q.size()) {
            os << ", " << q.top();
            q.pop();
        }
    }
    os << "]";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& st) {
    std::multiset<T> _st{st.begin(), st.end()};
    return os << _st;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::multiset<T>& st) {
    os << "{";
    if (st.size()) {
        os << *st.begin();
        for (auto it = ++st.begin(); it != st.end(); ++it)
            os << ", " << *it;
    }
    os << "}";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& st) {
    std::multiset<T> _st{st.begin(), st.end()};
    return os << _st;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unordered_multiset<T>& st) {
    std::multiset<T> _st{st.begin(), st.end()};
    return os << _st;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::map<T, U>& mp) {
    std::multimap<T, U> _mp{mp.begin(), mp.end()};
    return os << _mp;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<T, U>& mp) {
    std::multimap<T, U> _mp{mp.begin(), mp.end()};
    return os << _mp;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::multimap<T, U>& mp) {
    os << "{";
    if (mp.size()) {
        os << mp.begin()->first << ": " << mp.begin()->second;
        for (auto it = ++mp.begin(); it != mp.end(); ++it)
            os << ", " << it->first << ": " << it->second;
    }
    os << "}";
    return os;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::unordered_multimap<T, U>& mp) {
    std::multimap<T, U> _mp{mp.begin(), mp.end()};
    return os << _mp;
}

#endif
