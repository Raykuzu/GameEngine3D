//
// Created by raykuzu on 14/10/2020.
//

#pragma once

#include <map>
#include "Command.hpp"
#include "StringHelper.hpp"

class CommandResolver{
    public:
        CommandResolver() = default;
        ~CommandResolver() = default;

        static Command resolve(std::string const &line) {
            ArcLogger::trace("CommandResolver::resolve");
            ArcLogger::debug("Line: " + line);

            Command command = Command();
            std::vector<std::string> words = StringHelper::split(line, ' ');

            if (words.empty()) {
                ArcLogger::warn("Empty command string");
                return (command); // implicit none command
            }
            if (commandsMap.count(words.at(0)) == 0) {
                ArcLogger::warn("Unknown command: " + words.at(0));
                return (command); // same
            } else {
                if (commandsMap.at(words.at(0)).second != -1 && words.size() - 1 != (size_t)commandsMap.at(words.at(0)).second) {
                    ArcLogger::warn("Number of parameters mismatch for the command: " + words.at(0));
                    return (command); // same
                }
                command.setType(commandsMap.at(words.at(0)).first);
                for (size_t i = 1; i < words.size(); i++) {
                    command.addValue(words.at(i));
                }
            }
            return (command);
        }
    private:
        static std::map<std::string, std::pair<CommandType, ssize_t>> commandsMap;
};

std::map<std::string, std::pair<CommandType, ssize_t>> CommandResolver::commandsMap = {
        {"HELP", {CommandType::HELP, 0}},
        {"BASH", {CommandType::BASH, -1}},
        {"LOAD_GAME", {CommandType::LOAD_GAME, 1}},
        {"STOP", {CommandType::STOP, 0}},
        {"LOAD_CONF", {CommandType::LOAD_CONF, 1}}
};
