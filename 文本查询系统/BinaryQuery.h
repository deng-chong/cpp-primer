#ifndef BINARY_QUERY_H
#define BINARY_QUERY_H

#include "Query_base.h"
#include "Query.h"

class BinaryQuery : public Query_base {
protected:
    BinaryQuery(const Query& lq, const Query& rq, const std::string& s): lhs(lq), rhs(rq), op(s) {}
    std::string rep() const override { return "(" + lhs.rep() + op + rhs.rep() + ")"; }
    Query lhs, rhs;
    std::string op;
};

#endif