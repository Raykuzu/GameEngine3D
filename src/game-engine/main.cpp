//
// Created by raykuzu on 30/09/2020.
//

#include <csignal>
#include "Shell.hpp"

void sigHandler(__attribute__((unused)) int signum) {
    ArcLogger::info("Game engine teardown ...");
    ArcLogger::unsetLogLevel(ArcLogger::ERROR);
    Shell::running = false;
}

int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av) {
    ArcLogger::setBinaryName("GE3D");
    ArcLogger::setOutputDir("./log");
    ArcLogger::setLogLevel(ArcLogger::TRACE);
    ArcLogger::setLogLevel(ArcLogger::DEBUG);

    struct sigaction sgt {};

    sgt.sa_handler = &sigHandler;
    sgt.sa_flags = 0;
    sgt.sa_mask = {};
    sgt.sa_restorer = nullptr;

    sigaction(SIGHUP, &sgt, nullptr);
    sigaction(SIGINT, &sgt, nullptr);
    sigaction(SIGTERM, &sgt, nullptr);
    //signal(SIGPIPE, SIG_IGN); voir si c'est utile

    Shell shell;

    shell.run();
    return (0);
}