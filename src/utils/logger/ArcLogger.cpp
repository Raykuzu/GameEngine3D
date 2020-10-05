//
// Created by raykuzu on 30/09/2020.
//

#include "ArcLogger.hpp"

/* PRIVATE VARS */

Mutex ArcLogger::_logMutex;
Mutex ArcLogger:: _confMutex;
std::string ArcLogger::_binaryName = "binary";
std::string ArcLogger::_outputDir = "./";
bool ArcLogger::_logLevels[5] = {true, true, true, false, false};
bool ArcLogger::_fileRedirection = false;
std::ofstream ArcLogger::_ofs;

/* DESTRUCTOR */

ArcLogger::~ArcLogger()
{
    ArcLogger::deactivateFileRedirection();
}

/* PUBLIC FUNCTIONS */

/* LOGS FUNCTIONS */

void ArcLogger::log(std::string const &str, ArcLogger::LEVEL level)
{
    _logMutex.lock();
    if (_logLevels[(int)level] == 1) {
        if (!_fileRedirection)
            std::cout << _binaryName << ": " << getStrLogLevel(level) << ": " << str << std::endl;
        else {
            _ofs << _binaryName << " " << getCurrentTime() << " " << getStrLogLevel(level) << ": " << str << std::endl;
        }
    }
    _logMutex.unlock();
}

void ArcLogger::error(std::string const &str)
{
    ArcLogger::log(str, ArcLogger::ERROR);
}

void ArcLogger::warn(std::string const &str)
{
    ArcLogger::log(str, ArcLogger::WARN);
}

void ArcLogger::info(std::string const &str)
{
    ArcLogger::log(str, ArcLogger::INFO);
}

void ArcLogger::debug(std::string const &str)
{
    ArcLogger::log(str, ArcLogger::DEBUG);
}

void ArcLogger::trace(std::string const &str)
{
    ArcLogger::log(str, ArcLogger::TRACE);
}

/* CONF GETTERS & SETTERS */

void ArcLogger::setBinaryName(std::string const &binName)
{
    _confMutex.lock();
    _binaryName = binName;
    _confMutex.unlock();
}

std::string ArcLogger::getBinaryName()
{
    return (_binaryName);
}

void ArcLogger::setOutputDir(std::string const &outputDir)
{
    _confMutex.lock();
    _outputDir = outputDir;
    if (_outputDir.at(_outputDir.length() - 1) != '/')
        _outputDir += '/';
    _confMutex.unlock();
}

std::string ArcLogger::getOutputDir()
{
    return (_outputDir);
}

/* CONF LOGS LEVELS */

void ArcLogger::setLogLevel(ArcLogger::LEVEL level)
{
    _confMutex.lock();
    ArcLogger::_logLevels[((int)level)] = true;
    _confMutex.unlock();
}

void ArcLogger::unsetLogLevel(ArcLogger::LEVEL level)
{
    _confMutex.lock();
    ArcLogger::_logLevels[((int)level)] = false;
    _confMutex.unlock();
}

/* FILE REDIRECTION */

void ArcLogger::activateFileRedirection()
{
    _confMutex.lock();
    _fileRedirection = true;
    if (!_ofs.is_open())
        _ofs.open(_outputDir + "arcLogger_" + _binaryName + "_" +
        getCurrentTime() + ".log", std::ios::app);
    _confMutex.unlock();
}

void ArcLogger::deactivateFileRedirection()
{
    _confMutex.lock();
    _fileRedirection = false;
    if (_ofs.is_open())
        _ofs.close();
    _confMutex.unlock();
}

bool ArcLogger::isFileRedirectionActivated()
{
    return (_fileRedirection);
}

/* PRIVATE FUNCTIONS*/

/* CONF LOGS LEVELS */

std::string ArcLogger::getStrLogLevel(ArcLogger::LEVEL level)
{
    switch (level) {
        case ERROR:
            return ("ERROR");
        case WARN:
            return ("WARN");
        case INFO:
            return ("INFO");
        case DEBUG:
            return ("DEBUG");
        case TRACE:
            return ("TRACE");
    };
    return ("");
}

/* FILE MANAGEMENT */

std::string ArcLogger::getCurrentTime()
{
    char buff[64];
    time_t rawTime = 0;
    struct tm *now = nullptr;

    std::memset(buff, 0, 64);
    std::time(&rawTime);
    now = std::localtime(&rawTime);
    std::strftime(buff, 63, "%d-%m-%y_%X", now);
    return (std::string(buff));
}
