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

        }

        void term() override {

        }

        void update() override {

        }

private:
};
