#pragma once

#include <memory>
#include <vector>
#include <iostream>

using str = std::string;
using strv = std::vector<str>;
template<class T>
using uptr = std::unique_ptr<T>;

inline strv split(str const& s, char delim) {
    strv tokens;

    size_t word_start = 0;
    for (auto itr=s.begin(); itr!=s.end(); itr++) {
        if (*itr == delim) {
            tokens.push_back(std::string(s.begin()+word_start, itr));
            word_start = itr - s.begin() + 1;
            itr++;
        }
        else if (itr == s.end()-1) {
            tokens.push_back(std::string(s.begin()+word_start, itr+1));
        }
    }

    return tokens;
}
