//
// Created by raykuzu on 03/11/2020.
//

#pragma once

#include <functional>
#include "ATrigger.hpp"

template <typename Compare, typename Subject>
class AAutoTrigger : public ATrigger<Compare> {
    public:

        explicit AAutoTrigger(ITrigger::Type const &type, Compare const &compare, std::function<void (Subject &)> const &triggerFunc)
                            : ATrigger<Compare>(type, compare), _triggerFunc(triggerFunc) {};
        explicit AAutoTrigger(ITrigger::Type const &type, Compare const &compare, std::function<bool (Subject const &)> const &conditionFunc,
                            std::function<void (Subject &)> const &triggerFunc)
                            : ATrigger<Compare>(type, compare), _conditionFunc(conditionFunc), _triggerFunc(triggerFunc) {
            this->setCondition(true);
        };

        ~AAutoTrigger() override = default;


        void apply() override {
            if (this->isConditioned()) {
                if (!_conditionFunc(_subjectData))
                    return;
            }
            _triggerFunc(_subjectData);
            this->setTrigger(false);
        }

        void setSubject(Subject subjectData) {
            _subjectData = subjectData;
        }

    private:

        std::function<bool (Subject const &)> _conditionFunc {};
        std::function<void (Subject &)> _triggerFunc {};

        Subject _subjectData;
};