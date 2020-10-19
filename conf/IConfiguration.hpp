//
// Created by raykuzu on 14/10/2020.
//

#pragma once

#include "GlobalConfiguration.hpp"

class IConfiguration {
    public:
        virtual ~IConfiguration() = default;

        virtual GlobalConfiguration *getConfiguration() = 0;
};

extern "C" {
    GlobalConfiguration *getConfiguration();
}