//
// Created by raykuzu on 04/11/2020.
//

#pragma once

#include "ITrigger.hpp"

template <typename Compare>
class ATrigger : public ITrigger {
    public:

        explicit ATrigger(Type const &type, Compare compare) : _compare(compare) {};
        ~ATrigger() override = default;

        void trigger(Compare compare) {
            if (_compare == compare)
                _triggered = true;
        }

        [[nodiscard]] bool isTriggered() const override {
            return _triggered;
        }

        [[nodiscard]] bool isConditioned() const override {
            return _conditioned;
        }

        void setTrigger(bool trigger) override {
            _triggered = trigger;
        }

        void setCondition(bool condition) override {
            _conditioned = condition;
        }

        [[nodiscard]] Type getType() const {
            return _type;
        }


private:
        Type _type;
        Compare _compare;

        bool _triggered = false;
        bool _conditioned = false;

};
