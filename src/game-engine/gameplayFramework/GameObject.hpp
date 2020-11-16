//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <map>
#include <vector>
#include <functional>
#include <memory>
#include "ABiTrigger.hpp"
#include "ArcLogger.hpp"
#include "Uuid.hpp"
#include "transform.hh"
#include "collider.hh"
#include "functional"
#include "camera.hh"

static std::map<Component, std::function<component_p ()>> compMap = {
        {Component::TRANSFORM, transform_t::createComponent},
        {Component::COLLIDER, collider_t::createComponent},
        {Component::CAMERA, camera_t::createComponent}
};

class GameObject;

typedef std::shared_ptr<GameObject> sharedGO;

class GameObject {
    public:
        explicit GameObject(unsigned int flags) : _flags(flags), _alive(true) {
            if (flags == NO_FLAGS) {
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

        template<typename T>
        T getComponent(Component const &type) const {
            return (static_cast<T>(this->getComponent(type)));
        }

        template<typename T>
        void setComponent(Component const &type, T data) {
            this->getComponent(type)->assign(data);
        }

        void addTrigger(ITrigger *trigger) {
            _triggers.push_back(trigger);
        }

        [[nodiscard]] const std::vector<ITrigger *> &getTriggers() const {
            return _triggers;
        }

private:

        [[nodiscard]] component_p getComponent(Component const &type) const {
            for (auto component : _components) {
                if (component->_type == type)
                    return (component);
            }
            return (nullptr);
        }
        unsigned int _flags;
        std::string _id;
        bool _alive;

        std::vector<component_p> _components;

        std::vector<ITrigger *> _triggers;
};