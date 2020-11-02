//
// Created by raykuzu on 02/11/2020.
//

#pragma once

#include <utility>

#include "GameObject.hpp"

typedef void (*updateFunc)(sharedGO);

class Scene {
    public:
        explicit Scene(std::string id) : _id(std::move(id)) {};
        ~Scene() = default;

        [[nodiscard]] const std::string &getId() const {
            return _id;
        }

        void addGameObject(sharedGO const &gameObject) {
            gameObjects.push_back(gameObject);
        }


        // sale mais necessaire pour l'instant (voir la saloperie de constness des getters)
        std::vector<sharedGO> gameObjects;

    private:
        std::string _id;
};
