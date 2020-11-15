//
// Created by raykuzu on 02/11/2020.
//

#pragma once

#include <utility>
#include <ctime>
#include "GameObject.hpp"

class Scene {
    public:
        explicit Scene(std::string id) : _id(std::move(id)), _time(0, 0) {
            _changeScene.first = false;
        };
        ~Scene() = default;

        void clock() {
            _time.second = _time.first;
            _time.first = std::clock();
        }

        [[nodiscard]] const std::pair<clock_t, clock_t> &getTime() const {
            return _time;
        }

        [[nodiscard]] clock_t getTimeDiff() const {
            return (_time.second - _time.first);
        }

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

        // clock_t is long int
        std::pair<clock_t , clock_t> _time;

        std::pair<bool, std::string> _changeScene;

        std::vector<ITrigger *> _triggers;
};
