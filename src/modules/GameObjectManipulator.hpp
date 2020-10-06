//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include <algorithm>
#include "GameObject.hpp"

// Forbids the game dev to have these method exposed (via friend)

class GameObjectManipulator {
    public:
        static component_pointer createComponent(GameObject *gameObject, Component compType) {
            component_pointer comp = nullptr;

            for (auto compPair : compMap) {
                if (compType == compPair.first) {
                    comp = new component_t (compPair.second);
                    gameObject->_components.push_back(comp);
                    return (comp);
                }
            }
            throw std::exception();
        }

        static component_pointer deleteComponent(GameObject *gameObject, Component compType) {
            component_pointer comp = nullptr;

            for (auto compo : gameObject->_components) {
                if (compo->_type == compType) {
                    comp = compo;
                    gameObject->_components.erase(std::remove(gameObject->_components.begin(), gameObject->_components.end(), compo), gameObject->_components.end());
                    return (comp);
                }
            }
            return (nullptr);
        }
};
