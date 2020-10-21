//
// Created by raykuzu on 20/10/2020.
//

#pragma once

#include "ArcLogger.hpp"
#include "CommandResolver.hpp"
#include "Core.hpp"

class Shell {
    public:
        Shell() = default;
        ~Shell() = default;

        void run() {
            if (!_core.loadConf(DEFAULT_CONFIGURATION_PATH)) {
                ArcLogger::warn("Can't load default config please provide it later before running a game");
            } else {
                _core.configure();
            }
            ArcLogger::vanilla("Shell running, for information type HELP.");
            ArcLogger::vanilla("-----------------------------------------");
            std::string line;
            Command command;
            int exitStatus = 0;

            while (std::getline(std::cin, line)) {
                command = CommandResolver::resolve(line);
                switch (command.getType()) {
                    case NONE:
                        break;
                    case HELP:
                        printHelp();
                        break;
                    case LOAD_GAME:
                        if (!_core.hasConf()) {
                            ArcLogger::error("Please provide configuration before running a game");
                            continue;
                        }
                        _core.unloadGame();
                        if (_core.loadGame(command.getValues().at(0))) {
                            _core.teardownModuleManager();
                            _core.initGame();
                            _core.launchModuleManager();
                        }
                        break;
                    case LOAD_CONF:
                        _core.unloadConf();
                        if (_core.loadConf(command.getValues().at(0))) {
                            _core.teardownModuleManager();
                            _core.configure();
                        }
                        break;
                    case BASH:
                        //experimental
                        ArcLogger::vanilla(BashCommand::execCommand(StringHelper::merge(command.getValues(), ' '), exitStatus));
                        break;
                }
            }
            _core.teardownModuleManager();
        }

        static void printHelp() {
            ArcLogger::vanilla("Hello it's the help //TODO");
        }
    private:
        Core _core;

        static std::map<CommandType, std::function<void ()>> commandsMap;
};

std::map<CommandType, std::function<void ()>> Shell::commandsMap = {

};