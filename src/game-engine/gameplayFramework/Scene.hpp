//
// Created by raykuzu on 02/11/2020.
//

#pragma once

#include <utility>

#include "GameObject.hpp"

class Scene {
    public:
        explicit Scene(std::string id) : _id(std::move(id)) {
            _changeScene.first = false;
        };
        ~Scene() = default;

        [[nodiscard]] const std::string &getId() const {
            return _id;
        }

        void addGameObject(sharedGO const &gameObject) {
            gameObjects.push_back(gameObject);
        }

        void addTrigger(ITrigger *trigger) {
            _triggers.push_back(trigger);
        }

        [[nodiscard]] const std::vector<ITrigger *> &getTriggers() const {
            return _triggers;
        }

        [[nodiscard]] const std::pair<bool, std::string> &getChangeScene() const {
            return _changeScene;
        }

    // sale mais necessaire pour l'instant (voir la saloperie de constness des getters)
        std::vector<sharedGO> gameObjects;

    private:
        std::string _id;

        std::pair<bool, std::string> _changeScene;

        std::vector<ITrigger *> _triggers;
};
