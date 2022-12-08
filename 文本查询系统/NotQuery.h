#ifndef NOT_QUERY_H
#define NOT_QUERY_H

#include "Query_base.h"
#include "Query.h"
class Query;

class NotQuery : public Query_base {
    friend Query operator~(const Query&);
    NotQuery(const Query& q): query(q) {}
    QueryResult eval(const TextQuery&) const override;
    std::string rep() const override;
    Query query;
};

Query operator~(const Query& q) {
    return Query(std::shared_ptr<Query_base>(new NotQuery(q)));
}

QueryResult NotQuery::eval(const TextQuery& tq) const {
    auto res = query.eval(tq);
    auto ret_lines = std::make_shared<std::vector<line_no>>();
    line_no i = 0;
    for (auto j:*res.get_lines()) {
        while (i < j) ret_lines->push_back(i++);
        ++i;
    }
    auto sz = res.get_file()->size();
    while (i < sz) ret_lines->push_back(i++);
    return QueryResult(rep(), ret_lines, res.get_file());
}

std::string NotQuery::rep() const {
    return "~(" + query.rep() + ")";
}

#endif