#ifndef QUERY_H
#define QUERY_H

#include<iostream>
#include<string>
#include<memory>

#include "Query_base.h"
#include "QueryResult.h"
#include "TextQuery.h"
#include "WordQuery.h"

class Query {
    friend Query operator~(const Query&);
    friend Query operator&(const Query&, const Query&);
    friend Query operator|(const Query&, const Query&);
public:
    Query(const std::string& s): ptr(new WordQuery(s)) {};
    QueryResult eval(const TextQuery& tq) const {
        return ptr->eval(tq);
    }
    std::string rep() const {
        return ptr->rep();
    }
private:
    Query(std::shared_ptr<Query_base> p): ptr(p) {};
    std::shared_ptr<Query_base> ptr;
};

inline
std::ostream& operator<<(std::ostream& os, const Query& q) {
    return os << q.rep();
}

#endif