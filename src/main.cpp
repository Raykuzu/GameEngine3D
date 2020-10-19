//
// Created by raykuzu on 30/09/2020.
//
#include <iostream>

#include "Core.hpp"

int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av) {
    ArcLogger::setBinaryName("GE3D");
    ArcLogger::setOutputDir("./log");
    ArcLogger::setLogLevel(ArcLogger::TRACE);
    ArcLogger::setLogLevel(ArcLogger::DEBUG);
    Core core = Core();

    core.run();
    return (0);
}