//
// Created by raykuzu on 14/10/2020.
//

#include "ArcLogger.hpp"
#include "IConfiguration.hpp"

class Configuration : public IConfiguration {

    public:
        Configuration() {
            ArcLogger::setBinaryName("GE3D-Conf");
            ArcLogger::setOutputDir("./log");
            ArcLogger::setLogLevel(ArcLogger::TRACE);
            ArcLogger::setLogLevel(ArcLogger::DEBUG);

            _conf.addModule(ModuleConfiguration("physics", 1, {}));
        };
        ~Configuration() override = default;

        [[nodiscard]] GlobalConfiguration const &getConfiguration() override {
            ArcLogger::trace("Configuration::getConfiguration");
            return (_conf);
        }
    private:
        GlobalConfiguration _conf;
};

extern "C" {
    IConfiguration *loadConf() {
        ArcLogger::trace("Extern loadConf");
        return (new Configuration());
    }

    void unloadConf(IConfiguration *configuration) {
        ArcLogger::trace("Extern unloadConf");
        delete configuration;
    }
}