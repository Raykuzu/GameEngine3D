//
// Created by raykuzu on 30/09/2020.
//

#pragma once

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

        void registerComponent(component_pointer component) {
            ArcLogger::trace("AModule::registerComponent");
            _components.push_back(component);
        }

        void unregisterComponent(component_pointer component) {
            ArcLogger::trace("AModule::unregisterComponent");
            _components.erase(std::remove(_components.begin(), _components.end(), component), _components.end());
        }

        virtual void init() = 0;

        virtual void term() = 0;

        virtual void update() = 0;

    protected:
        std::vector<component_pointer> _components;

    private:
        ModuleConfiguration _moduleConfiguration;
};

