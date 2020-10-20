//
// Created by raykuzu on 14/10/2020.
//

#include "IConfiguration.hpp"

class Configuration : public IConfiguration {

    public:
        Configuration() {
            _conf.addModule(ModuleConfiguration("physics", 1, {}));

        };
        ~Configuration() override = default;

        [[nodiscard]] GlobalConfiguration const &getConfiguration() override {
            return (_conf);
        }
    private:
        GlobalConfiguration _conf;
};

extern "C" {
    IConfiguration *loadConf() {
        return (new Configuration());
    }

    void unloadConf(IConfiguration *configuration) {
        delete configuration;
    }
}