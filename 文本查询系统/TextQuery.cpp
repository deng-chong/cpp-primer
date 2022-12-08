#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<memory>

#include "TextQuery.h"
#include "QueryResult.h"

TextQuery::TextQuery(std::ifstream& ifs) {
    text = std::make_shared<std::vector<std::string>>();
    std::string s, t;
    line_no num = 0;
    while (getline(ifs, s)) {
        text->emplace_back(s);
        s += ' '; // 哨兵
        for (auto c:s) {
            if (c != ' ') t += c;
            else if (!t.empty()) {
                if (!mp.count(t)) mp[t] = std::make_shared<std::vector<line_no>>();
                if (mp[t]->empty() || mp[t]->back() != num)
                    mp[t]->push_back(num);
                t.clear();
            }
        }
        ++num;
    }
}

QueryResult TextQuery::query(const std::string& sought) const {
    if (!mp.count(sought)) return QueryResult(sought);
    return QueryResult(sought, mp.at(sought), text);
}