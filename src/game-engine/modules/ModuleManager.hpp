//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <vector>
#include <algorithm>
#include <future>
#include <cassert>
#include <UnixWindow.hpp>
#include "ArcLogger.hpp"
#include "GlobalConfiguration.hpp"
#include "PhysicsModule.hpp"
#include "TriggerModule.hpp"
#include "InputModule.hpp"
#include "IWindow.hpp"
#include "GraphicModule.hpp"

class ModuleManager {
    public:
        ModuleManager() {
        #ifdef _WIN32
            _window = new WinWindow();
        #endif // _WIN32

        #ifdef __linux__
            _window = new UnixWindow();
        #endif // _linux

            WindowSettings windowSettings;
            _window->create(windowSettings);
            _window->expose();

            availableModules["trigger"] = {new TriggerModule(), false};
            availableModules["physics"] = {new PhysicsModule(), false};
            availableModules["input"] = {new InputModule(_window), false};

            GraphicModule *graphicModule = new GraphicModule(_window);
            graphicModule->init();
    
            availableModules["graphic"] = {graphicModule, false};
        };
        ~ModuleManager() = default;

        void destroyModules() {
            ArcLogger::trace("ModuleManager::destroyModules");
            for (auto &availableMod : availableModules) {
                availableMod.second.second = false;
                delete availableMod.second.first;
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
            assert(getModuleByName("trigger") != nullptr);

            while (_run && futureExit.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
                Scene &scene = getSceneById(_actualScene);
                if (scene.exit())
                    _run = false;
                if (scene.getChangeScene().first) {
                    _actualScene = getSceneById(_actualScene).getChangeScene().second;
                }
                scene.clock();
                for (auto module : _modules) {
                    module->update(getSceneById(_actualScene));
                }

            }
        }

        void registerScene(Scene const &scene) {
            _scenes.push_back(scene);
        }

        Scene &getScene(std::string const &sceneId) {
            return (getSceneById(sceneId));
        }

        void registerGameObject(std::string const &sceneId, sharedGO const &gameObject) {
            ArcLogger::trace("ModuleManager::registerGameObject");
            std::cout << "registerGameObject: " << sceneId << std::endl;
            getSceneById(sceneId).addGameObject(gameObject);
            _gameObjects.push_back(gameObject);
        }

    private:
        AModule *getModuleByName(std::string const &name) {
            for (auto const &module : _modules) {
                if (name == module->getModuleConfiguration().getName()) {
                    return (module);
                }
            }
            throw std::runtime_error("Module [" + name + "] doesn't exist");
        }

        Scene &getSceneById(std::string const &id) {
            for (auto& scene : _scenes) {
                if (id == scene.getId()) {
                    return (scene);
                }
            }
            throw std::runtime_error("Scene [" + id + "] doesn't exist");
        }
        bool _run = true;

        GlobalConfiguration _configuration;
        std::vector<AModule *> _modules;
        std::vector<sharedGO> _gameObjects;

        std::vector<Scene> _scenes;
        std::string _actualScene = "scene1";

        IWindow *_window;

        std::map<std::string, std::pair<AModule *, bool>> availableModules;
};

