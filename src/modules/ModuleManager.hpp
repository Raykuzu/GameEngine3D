//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <vector>
#include <algorithm>
#include "ArcLogger.hpp"
#include "GlobalConfiguration.hpp"
#include "PhysicsModule.hpp"

class ModuleManager {
    public:
        ModuleManager() = default;
        ~ModuleManager() = default;

        void configure(GlobalConfiguration const &configuration) {

            std::pair<Module *, bool> modulePair;

            this->_configuration = configuration;
            for (auto const &conf : this->_configuration.getModules())
                if (availableModules.count(conf.first) == 0) {
                    ArcLogger::warn("Module [" + conf.first + "] doesn't exist");
                } else {
                    modulePair = availableModules[conf.first];
                    if (modulePair.second) {
                        ArcLogger::warn("Module [" + conf.first + "] already registered");
                    } else {
                        modulePair.first->configure(conf.second);
                        _modules.push_back(modulePair.first);
                    }
                }
            std::sort(_modules.begin(), _modules.end());
        }

    private:
        static std::map<std::string, std::pair<Module *, bool>> availableModules;

        GlobalConfiguration _configuration;
        std::vector<Module *> _modules;
};

std::map<std::string, std::pair<Module *, bool>> ModuleManager::availableModules = {
        {"physics", {new PhysicsModule(), false}},
};