#pragma once
#include <string>
#include <vector>
#include <regex>

struct Match {
    std::string type;
    std::string value;
    int position;
    int length;
    Match(std::string t, std::string v, int p, int l)
        : type(t), value(v), position(p), length(l) {
    }
};

std::vector<Match> analyzeText(const std::string& text);
