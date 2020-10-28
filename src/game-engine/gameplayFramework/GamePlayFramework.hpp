//
// Created by raykuzu on 19/10/2020.
//

#pragma once

#include "ModuleManager.hpp"

class GamePlayFramework {
    public:
        explicit GamePlayFramework(ModuleManager &moduleManager) : _moduleManager(moduleManager) {}
        ~GamePlayFramework() = default;

        sharedGO createGameObject(unsigned int flags) {
            std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(flags);
            _moduleManager.registerGameObject(gameObject);
            return (gameObject);
        }

        //void deleteGameObject(GameObject *gameObject) {
        //    _moduleManager.unregisterGameObject(gameObject);
        //}
    private:
        ModuleManager &_moduleManager;
};
