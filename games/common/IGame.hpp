//
// Created by raykuzu on 14/10/2020.
//

#pragma once

#include "GamePlayFramework.hpp"

class IGame {
    public:
        virtual void init(GamePlayFramework gamePlayFramework) = 0;

        virtual void term(GamePlayFramework gamePlayFramework) = 0;

        virtual void update(GamePlayFramework gamePlayFramework) = 0;
};

extern "C" {
    IGame *loadGame();
}
