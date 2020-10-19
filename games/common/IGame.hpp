//
// Created by raykuzu on 14/10/2020.
//

#pragma once

class IGame {
    public:
    virtual void init() = 0;

    virtual void term() = 0;

    virtual void update() = 0;
};
