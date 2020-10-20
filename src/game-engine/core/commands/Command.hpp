//
// Created by raykuzu on 14/10/2020.
//

#pragma once

#include <string>
#include <vector>

enum CommandType {
    NONE,
    LOAD
};

class Command {
    public:
        Command(): _type(CommandType::NONE) {};
        ~Command() = default;

        [[nodiscard]] CommandType getType() const {
            return _type;
        }

        void setType(CommandType type) {
            _type = type;
        }

        [[nodiscard]] const std::vector<std::string> &getValues() const {
            return _values;
        }

        void addValue(std::string const &value) {
            _values.push_back(value);
        }

        void setValues(const std::vector<std::string> &values) {
            _values = values;
        }

    private:
        CommandType _type;
        std::vector<std::string> _values;
};
