//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <vector>
#include <algorithm>
#include "ArcLogger.hpp"
#include "GlobalConfiguration.hpp"
#include "PhysicsModule.hpp"
#include "GameObjectManipulator.hpp"

class ModuleManager {
    public:
        ModuleManager() = default;
        ~ModuleManager() = default;

        void configure(GlobalConfiguration const &configuration) {

            std::pair<Module *, bool> modulePair;

            this->_configuration = configuration;
            for (auto const &conf : this->_configuration.getModules())
                if (availableModules.count(conf.first) == 0) {
                    ArcLogger::warn("Module [" + conf.first + "] doesn't exist");
                } else {
                    modulePair = availableModules[conf.first];
                    if (modulePair.second) {
                        ArcLogger::warn("Module [" + conf.first + "] already registered");
                    } else {
                        modulePair.first->configure(conf.second);
                        _modules.push_back(modulePair.first);
                    }
                }
            std::sort(_modules.begin(), _modules.end());
        }

        [[noreturn]] void run() { // temporary infinite
            while (true) {
                for (auto module : _modules) {
                    module->update();
                }

            }
        }

        GameObject *registerGameObject(unsigned int componentsFlags) {
            if (!(componentsFlags & Component::TRANSFORM) /*&& !(componentFlags & COMP)*/) {// And here
                ArcLogger::error("GameObject need one or more components");
                return (nullptr);
            }

            GameObject *gameObject = new GameObject(componentsFlags);
            component_pointer componentPointer = nullptr;

            for (auto compPair : compMap) {
                if (componentsFlags & compPair.first) {
                    componentPointer = GameObjectManipulator::createComponent(gameObject, compPair.first);//gameObject->createComponent(compPair.first);
                    getModuleByName(nameBinaryFlagModulesBridge[compPair.first])->registerComponent(componentPointer);
                }
            }
            _gameObjects.push_back(gameObject);
            return (gameObject);
        }

        void unregisterGameObject(GameObject *gameObject) {

            component_pointer componentPointer = nullptr;

            for (auto compPair : compMap) {
                if (gameObject->getFlags() & compPair.first) {
                    componentPointer = GameObjectManipulator::deleteComponent(gameObject, compPair.first);
                    getModuleByName(nameBinaryFlagModulesBridge[compPair.first])->unregisterComponent(componentPointer);
                }
            }
            _gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), gameObject), _gameObjects.end());
        }


    private:
        Module *getModuleByName(std::string const &name) {
            for (auto module : _modules) {
                if (name == module->getModuleConfiguration().getName()) {
                    return (module);
                }
            }
            return (nullptr);
        }

        static std::map<std::string, std::pair<Module *, bool>> availableModules;

        static std::map<unsigned int, std::string> nameBinaryFlagModulesBridge;

        GlobalConfiguration _configuration;
        std::vector<Module *> _modules;
        std::vector<GameObject *> _gameObjects;


        friend class GameObject;
};

std::map<std::string, std::pair<Module *, bool>> ModuleManager::availableModules = {
        {"physics", {new PhysicsModule(), false}},
};

std::map<unsigned int, std::string> ModuleManager::nameBinaryFlagModulesBridge = {
        {Component::TRANSFORM, "physics"},
};