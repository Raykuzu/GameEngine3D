//
// Created by raykuzu on 03/11/2020.
//

#pragma once

#include <functional>
#include "ATrigger.hpp"

template <typename Compare, typename Subject, typename Other>
class ABiTrigger : public ATrigger<Compare> {
    public:

        explicit ABiTrigger(ITrigger::Type const &type, Compare const &compare, std::function<void (Subject &, Other &)> const &triggerFunc)
                            : ATrigger<Compare>(type, compare), _triggerFunc(triggerFunc) {};
        explicit ABiTrigger(ITrigger::Type const &type, Compare const &compare, std::function<bool (Subject const &, Other const &)> const &conditionFunc,
                            std::function<void (Subject &, Other &)> const &triggerFunc)
                            : ATrigger<Compare>(type, compare), _conditionFunc(conditionFunc), _triggerFunc(triggerFunc) {
            this->setCondition(true);
        };

        ~ABiTrigger() override = default;

        void setSubject(Subject subjectData) {
            _subjectData = subjectData;
        }

        void setOther(Other otherData) {
            _otherData = otherData;
        }

        void apply() override {
            if (this->isConditioned()) {
                if (!_conditionFunc(_subjectData, _otherData))
                    return;
            }
            _triggerFunc(_subjectData, _otherData);
            this->setTrigger(false);
        }

    private:

        std::function<bool (Subject const &, Other const &)> _conditionFunc;
        std::function<void (Subject &, Other &)> _triggerFunc {};

        Subject _subjectData;
        Other _otherData;
};
