//
// Created by raykuzu on 20/10/2020.
//

#pragma once

#include <string>
#include <array>
#include <exception>

class BashCommand {
    public:
        static std::string execCommand(std::string const &cmd, int &exitStatus) {

            exitStatus = 0;
            auto pPipe = ::popen(cmd.c_str(), "r");
            if (pPipe == nullptr) {
                throw std::runtime_error("Cannot open pipe");
            }

            std::array<char, 256> buffer{};

            std::string result;

            while(not std::feof(pPipe)) {
                auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
                result.append(buffer.data(), bytes);
            }

            auto rc = ::pclose(pPipe);

            if(WIFEXITED(rc)) {
                exitStatus = WEXITSTATUS(rc);
            }

            return result;
        }
};