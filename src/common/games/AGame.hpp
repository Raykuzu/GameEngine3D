//
// Created by raykuzu on 14/10/2020.
//

#pragma once

#include "GamePlayFramework.hpp"

class IGame {
    public:
        explicit IGame(GamePlayFramework &gamePlayFramework) : _gamePlayFramework(gamePlayFramework) {};

        virtual ~IGame() = default;

        virtual void init() = 0;

        virtual void term() = 0;

        virtual void update() = 0;

    private:
        GamePlayFramework _gamePlayFramework;
};

extern "C" {
    IGame *loadGame();

    void unloadGame(IGame *game);
}
