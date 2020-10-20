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

class Core {
    public:
        Core() : _gamePlayFramework(_moduleManager) {};
        ~Core() = default;
        void run() {
            loadConfiguration();
            runShell();
        }

    private:
        void loadConfiguration() {
            if (_confLoader.load("./libconfiguration.so") == -1) {
                throw std::runtime_error("Can't load configuration");
            }
            _conf = ((IConfiguration * (*)())_confLoader.getFunction("loadConf"))();

            _moduleManager.configure(_conf->getConfiguration());
            ArcLogger::info(_conf->getConfiguration().getModules().at("physics").getName());
        }

        void runShell() {
            ArcLogger::vanilla("Core loaded .....");
            ArcLogger::vanilla("Shell running, for information type HELP.");
            ArcLogger::vanilla("-----------------------------------------");
            std::string line;
            Command command;

            while (std::getline(std::cin, line)) {
                command = CommandResolver::resolve(line);
                switch (command.getType()) {
                    case NONE:
                        break;
                    case HELP:
                        printHelp();
                        break;
                    case LOAD_GAME:
                        unloadGame();
                        loadGame(command.getValues().at(0));
                        teardownModuleManager();
                        _game->init();
                        configureAndRunModuleManager();
                        break;
                    case LOAD_CONF:
                        unloadConf();
                        loadConf(command.getValues().at(0));
                        teardownModuleManager();
                        configureAndRunModuleManager();
                        break;
                }
            }
        }

        void configureAndRunModuleManager() {
            _moduleManager.configure(_conf->getConfiguration());
            _moduleManagerTHandler.start(&ModuleManager::run, _moduleManager);
        }

        void teardownModuleManager() {
            _moduleManagerTHandler.stop();
            _moduleManager.destroy();
        }

        void loadGame(std::string const &path) {
            if (_gameLoader.load(path)) {
                ArcLogger::error("Can't load game at path [" + path + "]");
                return;
            }
            _game = ((AGame * (*)(GamePlayFramework &gamePlayFramework))_gameLoader.getFunction("loadGame"))(_gamePlayFramework);
        }

        void unloadGame() {
            if (_gameLoader.somethingLoaded()) {
                _game->term();
                ((void (*)(AGame *))_gameLoader.getFunction("unloadGame"))(_game);
                _gameLoader.unload();
            }
        }

        void loadConf(std::string const &path) {
            if (_confLoader.load(path)) {
                ArcLogger::error("Can't load conf at path [" + path + "]");
                return;
            }
            _conf = ((IConfiguration * (*)())_confLoader.getFunction("loadConf"))();
        }

        void unloadConf() {
            if (_confLoader.somethingLoaded()) {
                ((void (*)(IConfiguration *))_confLoader.getFunction("unloadConf"))(_conf);
                _confLoader.unload();
            }
        }

        static void printHelp() {
            ArcLogger::vanilla("Hello it's the help //TODO");
        }

        ModuleManager _moduleManager;
        ThreadHandler _moduleManagerTHandler;

        GamePlayFramework _gamePlayFramework;

        AGame *_game = nullptr;
        IConfiguration *_conf = nullptr;

        LibLoader _confLoader;
        LibLoader _gameLoader;
};
