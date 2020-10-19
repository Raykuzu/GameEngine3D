//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <GameObject.hpp>
#include "ModuleConfiguration.hpp"
#include "component.hh"

class AModule {
    public:
        AModule() = default;
        virtual ~AModule() = default;

        bool operator==(const AModule &rhs) const {
            return _moduleConfiguration.getOrder() == rhs._moduleConfiguration.getOrder();
        }

        bool operator!=(const AModule &rhs) const {
            return !(rhs == *this);
        }

        bool operator<(const AModule &rhs) const {
            return _moduleConfiguration.getOrder() < rhs._moduleConfiguration.getOrder();
        }

        bool operator>(const AModule &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const AModule &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const AModule &rhs) const {
            return !(*this < rhs);
        }

        void configure(ModuleConfiguration const &configuration) {
            this->_moduleConfiguration = configuration;
        }

        [[nodiscard]] const ModuleConfiguration &getModuleConfiguration() const {
            return _moduleConfiguration;
        }

        virtual void init() = 0;

        virtual void term() = 0;

        virtual void update(std::vector<GameObject *> gameObject) = 0;

    private:
        ModuleConfiguration _moduleConfiguration;
};

