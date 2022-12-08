#ifndef AND_QUERY_H
#define AND_QUERY_H

#include "BinaryQuery.h"

class AndQuery : public BinaryQuery {
    friend Query operator&(const Query&, const Query&);
    using BinaryQuery::BinaryQuery;
    QueryResult eval(const TextQuery&) const override;
};

Query operator&(const Query& lhs, const Query& rhs) {
    return Query(std::shared_ptr<Query_base>(new AndQuery(lhs, rhs, "&")));
}

QueryResult AndQuery::eval(const TextQuery& tq) const {
    auto left = lhs.eval(tq), right = rhs.eval(tq);
    auto ret_lines = std::make_shared<std::vector<line_no>>();
    auto i = left.get_lines()->begin(), j = right.get_lines()->begin();
    while (i != left.get_lines()->end() && j != right.get_lines()->end()) {
        if (*i == *j) ret_lines->push_back(*i), ++i, ++j;
        else if (*i < *j) ++i;
        else if (*i > *j) ++j;
    }
    return QueryResult(rep(), ret_lines, left.get_file());
}

#endif
