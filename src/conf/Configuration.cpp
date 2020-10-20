//
// Created by raykuzu on 14/10/2020.
//

#include "IConfiguration.hpp"

class Configuration : public IConfiguration {

    public:
        Configuration() = default;
        ~Configuration() override = default;

        GlobalConfiguration *getConfiguration() override {
            GlobalConfiguration *configuration = new GlobalConfiguration();

            configuration->addModule(ModuleConfiguration("physics", 1, {}));

            return (configuration);
        }
};

extern "C" {
    GlobalConfiguration *getConfiguration() {
        Configuration configuration;
        return (configuration.getConfiguration());
    }
}