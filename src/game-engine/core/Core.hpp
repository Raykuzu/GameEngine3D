//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "GamePlayFramework.hpp"
#include "LibLoader.hpp"
#include "CommandResolver.hpp"
#include "IConfiguration.hpp"
#include "AGame.hpp"
#include "ThreadHandler.hpp"
#include "BashCommand.hpp"

class Core {
    public:
        Core() : _gamePlayFramework(_moduleManager) {};
        ~Core() = default;

        /* Config */

        void configure() {
            ArcLogger::trace("Core::configure");
            /*if (_moduleManagerTHandler.started()) {
                _moduleManagerTHandler.stop();
            }
            _moduleManager.destroyModules();*/
            _configured = true;
            _moduleManager.configure(_conf->getConfiguration());
        }

        bool hasConf() {
            ArcLogger::trace("Core::hasConf");
            return (_conf != nullptr);
        }

        bool loadConf(std::string const &path) {
            ArcLogger::trace("Core::loadConf");
            if (_confLoader.load(path) == -1) {
                return (false);
            }
            _configured = true;
            return (checkAndExecute("loadConf"));
        }

        void unloadConf() {
            ArcLogger::trace("Core::unloadConf");
            if (_confLoader.somethingLoaded()) {
                checkAndExecute("unloadConf");
                _confLoader.unload();
            }
        }

        /* Game */

        bool loadGame(std::string const &path) {
            ArcLogger::trace("Core::loadGame");
            if (_gameLoader.load(path) == -1) {
                return (false);
            }
            return (checkAndExecute("loadGame"));
        }

        void unloadGame() {
            ArcLogger::trace("Core::unloadGame");
            if (_gameLoader.somethingLoaded()) {
                termGame();
                checkAndExecute("unloadGame");
                _gameLoader.unload();
            }
        }

        void initGame() {
            ArcLogger::trace("Core::initGame");
            _game->init();
        }

        void termGame() {
            ArcLogger::trace("Core::termGame");
            _game->term();
        }

        /* Module Manager */

        void launchModuleManager() {
            ArcLogger::trace("Core::launchModuleManager");
            if (_moduleManagerTHandler.started()) {
                return;
            }
            if (!_configured) {
                ArcLogger::error("Please configure before launch a game");
                return;
            }
            _moduleManagerTHandler.start(&ModuleManager::run, _moduleManager);
        }

        void teardownModuleManager() {
            ArcLogger::trace("Core::teardownModuleManager");
            if (_moduleManagerTHandler.started()) {
                _moduleManagerTHandler.stop();
            }
        }

        void cleanupModuleManager() {
            ArcLogger::trace("Core::cleanupModuleManager");
            if (_moduleManagerTHandler.started()) {
                ArcLogger::warn("Can't cleanup module manager while he is running");
                return;
            }
            _moduleManager.destroyModules();
        }

    private:
        bool checkAndExecute(std::string const &funcName) {
            void *func = nullptr;

            if (funcName == "loadConf" || funcName == "unloadConf") {
                func = _confLoader.getFunction(funcName);
            } else {
                func = _gameLoader.getFunction(funcName);
            }
            if (func == nullptr) {
                ArcLogger::error("Function [" + funcName + "] doesn't exist");
                return (false);
            }
            if (funcName == "loadConf") {
                _conf = ((IConfiguration * (*)()) func)();
            } else if (funcName == "unloadConf") {
                ((void(*)(IConfiguration *)) func)(_conf);
            } else if (funcName == "loadGame") {
                _game = ((AGame * (*)(GamePlayFramework &)) func)(_gamePlayFramework);
            } else if (funcName == "unloadGame") {
                ((void(*)(AGame *)) func)(_game);
            }
            return (true);
        }

        ModuleManager _moduleManager;
        ThreadHandler _moduleManagerTHandler;

        GamePlayFramework _gamePlayFramework;

        AGame *_game = nullptr;

        bool _configured = false;
        IConfiguration *_conf = nullptr;

        LibLoader _confLoader;
        LibLoader _gameLoader;
};
