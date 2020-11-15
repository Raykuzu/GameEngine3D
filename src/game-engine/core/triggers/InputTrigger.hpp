//
// Created by raykuzu on 03/11/2020.
//

#pragma once

#include "AAutoTrigger.hpp"
#include "GameObject.hpp"
#include "IWindow.hpp"

class InputTrigger : public AAutoTrigger<WindowInput, sharedGO> {
    public:
        explicit InputTrigger(WindowInput const &input, std::function<void (sharedGO &)> const &triggerFunc)
                              : AAutoTrigger(ITrigger::INPUT, input, triggerFunc) {};
        explicit InputTrigger(WindowInput const &input, std::function<bool (sharedGO const &)> const &conditionFunc,
                              std::function<void (sharedGO &)> const &triggerFunc)
                              : AAutoTrigger(ITrigger::INPUT, input, conditionFunc, triggerFunc) {};
        ~InputTrigger() override = default;
};
