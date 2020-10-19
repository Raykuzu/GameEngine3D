//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <map>
#include <vector>
#include "ArcLogger.hpp"
#include "Uuid.hpp"
#include "transform.hh"
#include "functional"

static std::map<Component, std::function<component_p ()>> compMap = {
        {Component::TRANSFORM, transform_t::createComponent}
};

class GameObject {
    public:
        explicit GameObject(unsigned int flags) : _flags(flags), _alive(true) {
            if (flags == 0x00) {
                ArcLogger::warn("GameObject need one or more components");
            }

            for (const auto& compPair : compMap) {
                if (flags & compPair.first) {
                    _components.push_back(compPair.second());
                }
            }
            _id = uuid::generate_uuid_v4();
        };
        virtual ~GameObject() {
            for (const auto& comp : _components) {
                delete comp;
            }
        };

        bool operator==(const GameObject &rhs) const {
            return _id == rhs._id;
        }

        bool operator!=(const GameObject &rhs) const {
            return !(rhs == *this);
        }

        [[nodiscard]] unsigned int getFlags() const {
            return _flags;
        }



    private:

        unsigned int _flags;
        std::string _id;
        bool _alive;
        std::vector<component_p> _components;
};