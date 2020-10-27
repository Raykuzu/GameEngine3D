//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <vector>
#include <algorithm>
#include <future>
#include "ArcLogger.hpp"
#include "GlobalConfiguration.hpp"
#include "PhysicsModule.hpp"

class ModuleManager {
    public:
        ModuleManager() = default;
        ~ModuleManager() {
            destroyGameObjects();
            destroyModules();
        };

        void destroyModules() {
            for (auto &availableMod : availableModules) {
                availableMod.second.second = false;
                delete availableMod.second.first;
            }
        }

        void destroyGameObjects() {
            for (auto const &gameObject : _gameObjects) {
                delete gameObject;
            }
        }

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
                        ArcLogger::debug("Module [" + conf.first + "] registered and configured");
                    }
                }
            std::sort(_modules.begin(), _modules.end());
        }

        void run(std::future<void> futureExit) {
            ArcLogger::trace("ModuleManager::run");
            while (futureExit.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
                for (auto module : _modules) {
                    module->update();
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
            throw std::runtime_error("Module [" + name + "] doesn't exist");
        }

        GlobalConfiguration _configuration;
        std::vector<AModule *> _modules;
        std::vector<GameObject *> _gameObjects;

        std::map<std::string, std::pair<AModule *, bool>> availableModules = {
                {"physics", {new PhysicsModule(_gameObjects), false}},
        };
};

