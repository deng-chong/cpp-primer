#ifndef TEXT_QUERY_H
#define TEXT_QUERY_H

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<memory>

class QueryResult;

class TextQuery {
public:
    using line_no = std::vector<std::string>::size_type;
    TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const;
private:
    std::shared_ptr<std::vector<std::string>> text;
    std::map<std::string, std::shared_ptr<std::vector<line_no>>> mp;
};


#endif