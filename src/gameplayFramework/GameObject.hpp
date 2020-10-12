//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <map>
#include <vector>
#include "ArcLogger.hpp"
#include "Uuid.hpp"
#include "transform.hh"

static std::map<Component, component_t> compMap = {
        {Component::TRANSFORM, transform_t {}}
};

class GameObjectManipulator;

class GameObject {
    public:
        explicit GameObject(unsigned int flags) : _flags(flags), _alive(true) {
            _id = uuid::generate_uuid_v4();
        };
        virtual ~GameObject() = default;

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
        friend class GameObjectManipulator;

        unsigned int _flags;
        std::string _id;
        bool _alive;
        std::vector<component_pointer> _components;
};