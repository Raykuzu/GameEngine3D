//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include <map>
#include <string>

class ModuleConfiguration {
    public:
        ModuleConfiguration() = default;
        ModuleConfiguration(std::string name, int order, std::map<std::string, std::string> parameters)
            : _name(std::move(name)), _order(order), _parameters(std::move(parameters)) {};
        ~ModuleConfiguration() = default;

        bool operator==(const ModuleConfiguration &rhs) const {
            return _name == rhs._name &&
                   _order == rhs._order &&
                   _parameters == rhs._parameters;
        }

        bool operator!=(const ModuleConfiguration &rhs) const {
            return !(rhs == *this);
        }

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