//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include "GlobalConfiguration.hpp"

class ConfParser {
    public:
        ConfParser() = default;
        ~ConfParser() = default;

        void parse(std::string const &rawContent, GlobalConfiguration &conf) {
            size_t i = 0;
            std::string rootW;

            while (isblank(rawContent[i]))
                i++;
            while ()
        }
    private:
};