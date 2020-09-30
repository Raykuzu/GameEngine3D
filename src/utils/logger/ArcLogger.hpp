/*
** EPITECH PROJECT, 2019
** Arcade
** File description:
** logger source
*/

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <ctime>

class ArcLogger {
	public:

        /* LOG LEVEL */

        enum LEVEL {
            ERROR,
            WARN,
            INFO,
            DEBUG,
            TRACE
        };

        /* DESTRUCTOR */

		~ArcLogger();

        /* LOGS FUNCTIONS */

        static void log(std::string const &, ArcLogger::LEVEL);
        static void error(std::string const &);
        static void warn(std::string const &);
        static void info(std::string const &);
        static void debug(std::string const &);
        static void trace(std::string const &);

        /* CONF GETTERS & SETTERS */

        static void setBinaryName(std::string const &);
        static std::string getBinaryName();
        static void setOutputDir(std::string const &);
        static std::string getOutputDir();

        /* CONF LOGS LEVELS */

        static void setLogLevel(ArcLogger::LEVEL);
        static void unsetLogLevel(ArcLogger::LEVEL);

        /* CONF FILE REDIRECTION */

        static void activateFileRedirection();
        static void desactivateFileRedirection();
        static bool isFileRedirectionActivated();

    private:
        static std::string getStrLogLevel(ArcLogger::LEVEL);
        static void fileLog(std::stringstream &);
        static std::string getCurrentTime();

        static Mutex _logMutex;
        static Mutex _confMutex;
        static std::string _binaryName;
        static std::string _outputDir;
        static bool _logLevels[5];
        static bool _fileRedirection;
        static std::ofstream _ofs;
};