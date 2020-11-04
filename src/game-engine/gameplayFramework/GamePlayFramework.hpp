//
// Created by raykuzu on 19/10/2020.
//

#pragma once

#include "ModuleManager.hpp"

class GamePlayFramework {
    public:
        explicit GamePlayFramework(ModuleManager &moduleManager) : _moduleManager(moduleManager) {}
        ~GamePlayFramework() = default;

        void createScene(std::string const &id) {
            _moduleManager.registerScene(Scene(id));
        }

        void addTriggerToScene(std::string const &id, ITrigger *trigger) {
            _moduleManager.getScene(id).addTrigger(trigger);
        }

        sharedGO createGameObject(std::string const &sceneId, unsigned int flags) {
            sharedGO gameObject = std::make_shared<GameObject>(flags);
            _moduleManager.registerGameObject(sceneId, gameObject);
            return (gameObject);
        }

        //void deleteGameObject(GameObject *gameObject) {
        //    _moduleManager.unregisterGameObject(gameObject);
        //}
    private:
        ModuleManager &_moduleManager;
};
