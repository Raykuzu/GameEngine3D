//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include "ModuleConfiguration.hpp"

class Module {
    public:
        Module() = default;
        virtual ~Module() = default;

        bool operator==(const Module &rhs) const {
            return _moduleConfiguration.getOrder() == rhs._moduleConfiguration.getOrder();
        }

        bool operator!=(const Module &rhs) const {
            return !(rhs == *this);
        }

        bool operator<(const Module &rhs) const {
            return _moduleConfiguration.getOrder() < rhs._moduleConfiguration.getOrder();
        }

        bool operator>(const Module &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const Module &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const Module &rhs) const {
            return !(*this < rhs);
        }

        void configure(ModuleConfiguration const &configuration) {
            this->_moduleConfiguration = configuration;
        }

        virtual void update() = 0;


    private:
        ModuleConfiguration _moduleConfiguration;
};

