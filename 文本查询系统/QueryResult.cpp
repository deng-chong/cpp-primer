#include "QueryResult.h"

std::shared_ptr<const std::vector<TextQuery::line_no>> QueryResult::get_lines() const {
    return lines;
}

std::shared_ptr<const std::vector<std::string>> QueryResult::get_file() const {
    return text;
}

void QueryResult::show() const {
    if (!lines) {
        std::cout << "std::string " << sought << " not found in the text.";
    } else {
        std::cout << "\"" << sought << "\" occurs " << (*lines).size() << " time" << ((*lines).size()>1?"s":"") << "." << std::endl;
        for (auto num:*lines) 
            std::cout << "Line " << num+1 << ": " << (*text)[num] << std::endl;
    }
}