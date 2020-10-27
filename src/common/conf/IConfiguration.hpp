//
// Created by raykuzu on 14/10/2020.
//

#pragma once

#include "GlobalConfiguration.hpp"

#define DEFAULT_CONFIGURATION_PATH "./libconf.so"

class IConfiguration {
    public:
        virtual ~IConfiguration() = default;

        virtual GlobalConfiguration const &getConfiguration() = 0;
};

extern "C" {
    IConfiguration *loadConf();

    void unloadConf(IConfiguration *configuration);
}