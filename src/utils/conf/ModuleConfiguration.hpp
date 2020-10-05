//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include <map>
#include <string>

class ModuleConfiguration {
    public:
        ModuleConfiguration() = default;
        ~ModuleConfiguration() = default;

        [[nodiscard]] const std::string &getName() const {
            return _name;
        }

        void setName(const std::string &name) {
            _name = name;
        }

        [[nodiscard]] int getOrder() const {
            return _order;
        }

        void setOrder(int order) {
            _order = order;
        }

        [[nodiscard]] const std::map<std::string, std::string> &getParameters() const {
            return _parameters;
        }

    private:
        std::string _name;
        int _order = 0;
        std::map<std::string, std::string> _parameters;

};