//
// Created by raykuzu on 30/09/2020.
//

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <mutex>

typedef std::mutex Mutex;

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

        static void vanilla(std::string const &);

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
        static void deactivateFileRedirection();
        static bool isFileRedirectionActivated();

    private:
        static std::string getStrLogLevel(ArcLogger::LEVEL);
        static std::string getCurrentTime();

        static Mutex _logMutex;
        static Mutex _confMutex;
        static std::string _binaryName;
        static std::string _outputDir;
        static bool _logLevels[5];
        static bool _fileRedirection;
        static std::ofstream _ofs;
};