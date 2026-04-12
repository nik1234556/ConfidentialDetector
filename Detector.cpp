#include "pch.h"
#include "Detector.h"
#include <cctype>
#include <regex>

static bool isBankCard(const std::string& text, int start, int end) {
    std::string digits;
    for (int i = start; i < end && i < (int)text.length(); ++i) {
        if (std::isdigit((unsigned char)text[i]))
            digits += text[i];
    }
    return digits.length() == 16;
}

static bool isPassport(const std::string& text, int start, int end) {
    std::string digits;
    for (int i = start; i < end && i < (int)text.length(); ++i) {
        if (std::isdigit((unsigned char)text[i]))
            digits += text[i];
    }
    return digits.length() == 10;
}

static bool isContractNumber(const std::string& text, int start, int end) {
    std::string clean;
    for (int i = start; i < end && i < (int)text.length(); ++i) {
        if (std::isalnum((unsigned char)text[i]))
            clean += text[i];
    }
    return (clean.length() >= 5 && clean.length() <= 20);
}

std::vector<Match> analyzeText(const std::string& text) {
    std::vector<Match> results;

    try {
        std::regex bankCardPattern(R"((\d{4}[-\s]?){3}\d{4})");
        auto words_begin = std::sregex_iterator(text.begin(), text.end(), bankCardPattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string value = match.str();
            int pos = (int)match.position();
            if (isBankCard(text, pos, pos + (int)match.length())) {
                results.push_back(Match("Банковская карта", value, pos, (int)match.length()));
            }
        }
    }
    catch (const std::regex_error&) {}

    try {
        std::regex passportPattern(R"(\d{2}[-\s]?\d{2}[-\s]?\d{6})");
        auto words_begin = std::sregex_iterator(text.begin(), text.end(), passportPattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string value = match.str();
            int pos = (int)match.position();
            if (isPassport(text, pos, pos + (int)match.length())) {
                results.push_back(Match("Паспортные данные", value, pos, (int)match.length()));
            }
        }
    }
    catch (const std::regex_error&) {}

    try {
        std::regex contractPattern(R"((?:договор|ДОГОВОР|contract|дор|ДОГ|№)[-\s]?[\w-]+)",
            std::regex::icase);
        auto words_begin = std::sregex_iterator(text.begin(), text.end(), contractPattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string value = match.str();
            int pos = (int)match.position();
            if (isContractNumber(text, pos, pos + (int)match.length())) {
                results.push_back(Match("Номер договора", value, pos, (int)match.length()));
            }
        }
    }
    catch (const std::regex_error&) {}

    return results;
}