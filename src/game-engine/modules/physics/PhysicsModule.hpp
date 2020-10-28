//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include "AModule.hpp"

class PhysicsModule : public AModule {
    public:
        explicit PhysicsModule(std::vector<sharedGO> &gameObjects) : AModule(gameObjects) {};
        ~PhysicsModule() override = default;

        void init() override {
            ArcLogger::trace("PhysicsModule::init");
        }

        void term() override {
            ArcLogger::trace("PhysicsModule::term");
        }

        void update() override {
            //ArcLogger::trace("PhysicsModule::update");
        }

    private:
};
