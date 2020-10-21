//
// Created by raykuzu on 14/10/2020.
//

#pragma once

#include "GamePlayFramework.hpp"

class AGame {
    public:
        explicit AGame(GamePlayFramework &gamePlayFramework) : _gamePlayFramework(gamePlayFramework) {};
        virtual ~AGame() = default;

        virtual void init() = 0;

        virtual void term() = 0;

        virtual void update() = 0;

    protected:
        GamePlayFramework &_gamePlayFramework;
};

extern "C" {
    AGame *loadGame(GamePlayFramework &gamePlayFramework);

    void unloadGame(AGame *game);
}
