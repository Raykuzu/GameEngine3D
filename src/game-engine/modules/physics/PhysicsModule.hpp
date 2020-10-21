//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include "AModule.hpp"

class PhysicsModule : public AModule {
    public:
        explicit PhysicsModule(std::vector<GameObject *> &gameObjects) : AModule(gameObjects) {};
        ~PhysicsModule() override = default;

        void init() override {
            ArcLogger::debug("PhysicsModule::init");
        }

        void term() override {
            ArcLogger::debug("PhysicsModule::term");
        }

        void update() override {
            ArcLogger::debug("PhysicsModule::update");
        }

    private:
};
