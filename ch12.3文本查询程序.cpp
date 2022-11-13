#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<memory>
using namespace std;


class QueryResult;

class TextQuery {
public:
    using line_no = vector<string>::size_type;
    TextQuery(ifstream&);
    QueryResult query(const string&) const;
private:
    shared_ptr<vector<string>> text;
    map<string, shared_ptr<vector<line_no>>> mp;
};

TextQuery::TextQuery(ifstream& ifs) {
    text = make_shared<vector<string>>();
    string s, t;
    line_no num = 0;
    while (getline(ifs, s)) {
        text->emplace_back(s);
        s += ' '; // 哨兵
        for (auto c:s) {
            if (c != ' ') t += c;
            else if (!t.empty()) {
                if (!mp.count(t)) mp[t] = make_shared<vector<line_no>>();
                if (mp[t]->empty() || mp[t]->back() != num)
                    mp[t]->push_back(num);
                t.clear();
            }
        }
        ++num;
    }
}

class QueryResult {
public:
    QueryResult(const string& s): sought(s) {};
    QueryResult(const string& s, shared_ptr<vector<TextQuery::line_no>> p, shared_ptr<vector<string>> t):
        sought(s), lines(p), text(t) {}
    void show() const {
        if (!lines) {
            cout << "String " << sought << " not found in the text.";
        } else {
            cout << "\"" << sought << "\" occurs " << (*lines).size() << " time" << ((*lines).size()>1?"s":"") << "." << endl;
            for (auto num:*lines) 
                cout << "Line " << num+1 << ": " << (*text)[num] << endl;
        }
    }
private:
    string sought;
    shared_ptr<vector<TextQuery::line_no>> lines;
    shared_ptr<vector<string>> text;
};

QueryResult TextQuery::query(const string& sought) const {
    if (!mp.count(sought)) return QueryResult(sought);
    return QueryResult(sought, mp.at(sought), text);
}


int main() {
    ifstream ifs("news.txt");
    TextQuery tq(ifs);
    QueryResult qr = tq.query("Friday");
    qr.show();
}
