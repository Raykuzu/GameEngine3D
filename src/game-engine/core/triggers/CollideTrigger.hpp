//
// Created by raykuzu on 04/11/2020.
//

#pragma once

#include "ABiTrigger.hpp"
#include "GameObject.hpp"

class CollideTrigger : public ABiTrigger<bool, sharedGO, sharedGO>{
    public:
        explicit CollideTrigger(std::function<void (sharedGO &, sharedGO &)> const &triggerFunc)
                       : ABiTrigger(ITrigger::COLLIDE, true, triggerFunc){};
        CollideTrigger(std::function<bool (sharedGO const &, sharedGO const &)> const &conditionFunc,
                       std::function<void (sharedGO &, sharedGO &)> const &triggerFunc)
                       : ABiTrigger(ITrigger::COLLIDE, true, conditionFunc, triggerFunc){};
        ~CollideTrigger() override = default;
    private:
};
