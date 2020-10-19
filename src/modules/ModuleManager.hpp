//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <vector>
#include <algorithm>
#include "ArcLogger.hpp"
#include "GlobalConfiguration.hpp"
#include "PhysicsModule.hpp"

class ModuleManager {
    public:
        ModuleManager() = default;
        ~ModuleManager() {
            for (auto const &availableMod : availableModules) {
                delete availableMod.second.first;
            }
        };

        void configure(GlobalConfiguration const &configuration) {
            ArcLogger::trace("ModuleManager::configure");

            std::pair<AModule *, bool> modulePair;

            this->_configuration = configuration;
            for (auto const &conf : this->_configuration.getModules())
                if (availableModules.count(conf.first) == 0) {
                    ArcLogger::warn("AModule [" + conf.first + "] doesn't exist");
                } else {
                    modulePair = availableModules[conf.first];
                    if (modulePair.second) {
                        ArcLogger::warn("AModule [" + conf.first + "] already registered");
                    } else {
                        modulePair.first->configure(conf.second);
                        _modules.push_back(modulePair.first);
                    }
                }
            std::sort(_modules.begin(), _modules.end());
        }

        [[noreturn]] void run() { // temporary infinite
            ArcLogger::trace("ModuleManager::run");
            while (true) {
                for (auto module : _modules) {
                    module->update(_gameObjects);
                }

            }
        }

        void registerGameObject(GameObject *gameObject) {
            ArcLogger::trace("ModuleManager::registerGameObject");

            _gameObjects.push_back(gameObject);
        }


        void unregisterGameObject(GameObject *gameObject) {
            ArcLogger::trace("ModuleManager::unregisterGameObject");

            _gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), gameObject), _gameObjects.end());
        }


    private:
        AModule *getModuleByName(std::string const &name) {
            for (auto module : _modules) {
                if (name == module->getModuleConfiguration().getName()) {
                    return (module);
                }
            }
            return (nullptr);
        }

        static std::map<std::string, std::pair<AModule *, bool>> availableModules;

        GlobalConfiguration _configuration;
        std::vector<AModule *> _modules;
        std::vector<GameObject *> _gameObjects;

        friend class GameObject;
};

std::map<std::string, std::pair<AModule *, bool>> ModuleManager::availableModules = {
        {"physics", {new PhysicsModule(), false}},
};
