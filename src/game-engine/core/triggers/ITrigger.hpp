//
// Created by raykuzu on 03/11/2020.
//

#pragma once

class ITrigger {
    public:
        enum Type {
            INPUT,
            COLLIDE
        };

        virtual ~ITrigger() = default;

        virtual void apply() = 0;

        [[nodiscard]] virtual bool isTriggered() const = 0;

        [[nodiscard]] virtual bool isConditioned() const = 0;

        virtual void setTrigger(bool trigger) = 0;

        virtual void setCondition(bool condition) = 0;
};