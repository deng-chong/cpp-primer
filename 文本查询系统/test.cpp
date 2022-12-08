#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<memory>

#include "TextQuery.h"
#include "QueryResult.h"
#include "Query.h"
#include "WordQuery.h"
#include "AndQuery.h"
#include "OrQuery.h"
#include "NotQuery.h"


int main() {
    std::ifstream ifs("news.txt");
    TextQuery tq(ifs);
    QueryResult qr = tq.query("the");
    qr.show();
    Query q1("a"), q2("the"), q3("he"), q = q1 & q2 | ~q3;
    qr = q.eval(tq);
    std::cout << q;
    qr.show();
}


