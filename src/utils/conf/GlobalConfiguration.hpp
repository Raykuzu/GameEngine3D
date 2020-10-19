//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include "ModuleConfiguration.hpp"

class GlobalConfiguration {
    public:
        GlobalConfiguration() = default;
        ~GlobalConfiguration() = default;

        [[nodiscard]] const std::map<std::string, ModuleConfiguration> &getModules() const {
            return _modules;
        }

        void addModule(ModuleConfiguration const &moduleConfiguration) {
            _modules.insert(std::make_pair(moduleConfiguration.getName(), moduleConfiguration));
        }

        void removeModule(std::string const &name) {
            _modules.erase(name);
        }

    private:
        std::map<std::string, ModuleConfiguration> _modules;
};