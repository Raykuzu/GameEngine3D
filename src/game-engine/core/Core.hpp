//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "GamePlayFramework.hpp"
#include "LibLoader.hpp"
#include "CommandResolver.hpp"
//#include "IGame.hpp"

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
            if (_confLoader.load("conf/conf.so") == -1) {
                throw std::runtime_error("Can't load configuration");
            }
            GlobalConfiguration *confPtr = ((GlobalConfiguration * (*)())_confLoader.getFunction("getConfiguration"))();
            GlobalConfiguration conf = GlobalConfiguration(*confPtr);

            delete confPtr;
            _moduleManager.configure(conf);
            ArcLogger::info(conf.getModules().at("physics").getName());
        }

        void runShell() {
            std::string line;
            Command command;

            while (std::getline(std::cin, line)) {
                command = CommandResolver::resolve(line);
                switch (command.getType()) {
                    case NONE:
                        break;
                    case LOAD:
                        if (_gameLoader.somethingLoaded()) {
                            //((IGame * (*)())_gameLoader.getFunction("loadGame"))(_game);
                            _gameLoader.unload();
                        }
                        if (_gameLoader.load(command.getValues().at(0))) {
                            continue;
                        }
                        // _game = ((IGame * (*)())_gameLoader.getFunction("loadGame"))();

                        break;
                }
            }
        }
        ModuleManager _moduleManager;
        GamePlayFramework _gamePlayFramework;

        //IGame *_game;

        LibLoader _confLoader;
        LibLoader _gameLoader;
};
