//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

class StringHelper {
	public:
        ~StringHelper() = default;

        static std::vector<std::string> split(std::string const &base, std::string const &sep) {
            std::vector<std::string> splited;
            size_t last = 0;
            size_t next = 0;

            while ((next = base.find(sep, last)) != std::string::npos) {
                splited.push_back(base.substr(last, next-last));
                last = next + sep.size();
            }
            splited.push_back(base.substr(last));
            return (splited);
        };

        static std::vector<std::string> split(std::string const &base, char const sep) {
            std::vector<std::string> splited;
            std::stringstream ss(base);

            for (std::string token; std::getline(ss, token, sep); splited.push_back(token));
            return (splited);
        };

        static std::string merge(std::vector<std::string> const &base, std::string const &sep) {
            std::string merged;

            for (auto const &word : base) {
                merged += word;
                merged += sep;
            }
            return (merged);
        }

        static std::string merge(std::vector<std::string> const &base, char const sep) {
            std::string merged;

            for (auto const &word : base) {
                merged += word;
                merged += sep;
            }
            return (merged);
        }

        static void trim(std::string &base) {
            std::replace(base.begin(), base.end(), '\t', ' ');
            for (size_t i = 0; i < base.size() - 1; i++) {
                if (base.at(i) == ' ' && base.at(i + 1) == ' ') {
                    base.erase(i, 1);
                    i--;
                }
            }
        };

        static void reduct(std::string &base) {
            for (size_t i = 0; i < base.size(); i++) {
                if (base.at(i) == ' ' || base.at(i) == '\t') {
                    base.erase(i, 1);
                    i--;
                } else {
                    break;
                }
            }
            for (size_t i = base.size() - 1; i > 0; i--) {
                if (base.at(i) == ' ' || base.at(i) == '\t') {
                    base.erase(i, 1);
                    i--;
                } else {
                    break;
                }
            }
        };

    private:
        StringHelper() = default;
};
