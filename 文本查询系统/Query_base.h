#ifndef QUERY_BASE_H
#define QUERY_BASE_H

#include "TextQuery.h"
#include "QueryResult.h"

class Query_base {//抽象基类
    friend class Query;
protected:
    using line_no = TextQuery::line_no;
    virtual ~Query_base() = default;
private:
    virtual QueryResult eval(const TextQuery& tq) const = 0;
    virtual std::string rep() const = 0;
};

#endif