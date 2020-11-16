//
// Created by raykuzu on 04/11/2020.
//

#pragma once

#include "AModule.hpp"

class TriggerModule : public AModule {
    public:
        TriggerModule() = default;
        ~TriggerModule() override = default;

        void init() override {

        }

        void term() override {

        }

        void update(Scene &scene) override {
            for (auto const &t : scene.getTriggers()) {
                if (t->getType() == ITrigger::INPUT) {
                    //dynamic_cast<ATrigger<WindowInput> *>(t)->trigger(scene.getLastInput());
                }
                if (t->isTriggered()) {
                    t->apply();
                }
            }
            for (auto const &obj : scene.gameObjects) {
                for (auto const &t : obj->getTriggers()) {
                    if (t->isTriggered()) {
                        t->apply();
                    }
                }
            }
        }
};
