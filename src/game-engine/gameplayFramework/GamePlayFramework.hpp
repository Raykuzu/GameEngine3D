//
// Created by raykuzu on 19/10/2020.
//

#pragma once

#include "ModuleManager.hpp"

class GamePlayFramework {
    public:
        explicit GamePlayFramework(ModuleManager &moduleManager) : _moduleManager(moduleManager) {}
        ~GamePlayFramework() = default;

        void registerGameObject(GameObject *gameObject) {
            _moduleManager.registerGameObject(gameObject);
        }

        void unregisterGameObject(GameObject *gameObject) {
            _moduleManager.unregisterGameObject(gameObject);
        }
    private:
        ModuleManager &_moduleManager;
};
