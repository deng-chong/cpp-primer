#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<memory>

#include "TextQuery.h"

class QueryResult {
public:
    QueryResult(const std::string& s): sought(s) {};
    QueryResult(const std::string& s, std::shared_ptr<std::vector<TextQuery::line_no>> p, std::shared_ptr<const std::vector<std::string>> t):
        sought(s), lines(p), text(t) {}
    std::shared_ptr<const std::vector<TextQuery::line_no>> get_lines() const;
    std::shared_ptr<const std::vector<std::string>> get_file() const;
    void show() const;
private:
    std::string sought;
    std::shared_ptr<std::vector<TextQuery::line_no>> lines; //行号严格单调递增
    std::shared_ptr<const std::vector<std::string>> text;
};


#endif