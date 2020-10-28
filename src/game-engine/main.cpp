//
// Created by raykuzu on 30/09/2020.
//

#ifndef _WIN32
    #include <csignal>
#endif
#include "PhysicsModule.hpp"
#include "Shell.hpp"

#ifndef _WIN32
void sigHandler(__attribute__((unused)) int signum) {
    ArcLogger::info("Game engine teardown ...");
    ArcLogger::unsetLogLevel(ArcLogger::ERROR);
    Shell::running = false;
}
#endif

int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av) {
    ArcLogger::setBinaryName("GE3D");
    ArcLogger::setOutputDir("./log");
    ArcLogger::setLogLevel(ArcLogger::TRACE);
    ArcLogger::setLogLevel(ArcLogger::DEBUG);

    #ifndef _WIN32
    struct sigaction sgt {};

    sgt.sa_handler = &sigHandler;
    sgt.sa_flags = 0;
    sgt.sa_mask = {};
    sgt.sa_restorer = nullptr;

    sigaction(SIGHUP, &sgt, nullptr);
    sigaction(SIGINT, &sgt, nullptr);
    sigaction(SIGTERM, &sgt, nullptr);
    //signal(SIGPIPE, SIG_IGN); voir si c'est utile
    #endif

    Shell shell;

    shell.run();
    return (0);
}