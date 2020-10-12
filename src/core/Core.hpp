//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "ModuleManager.hpp"

class Core {
    public:
        Core() = default;
        ~Core() = default;
        void run() {

        }

        GameObject *registerGameObject(unsigned int componentsFlags) {
            return (_moduleManager.registerGameObject(componentsFlags));
        }

        void unregisterGameObject(GameObject *gameObject) {
            _moduleManager.unregisterGameObject(gameObject);
        }
    private:
        ModuleManager _moduleManager;
};
