#ifndef WORD_QUERY_H
#define WORD_QUERY_H

#include "Query_base.h"

class WordQuery : public Query_base {
    friend class Query;
    WordQuery(const std::string& s): query_word(s) {}
    QueryResult eval(const TextQuery& tq) const override {
        return tq.query(query_word);
    }
    std::string rep() const override {
        return query_word;
    }
    std::string query_word;
};



#endif