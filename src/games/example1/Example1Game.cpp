//
// Created by raykuzu on 19/10/2020.
//

#include "AGame.hpp"

class Example1Game : public AGame {
    public:
        explicit Example1Game(GamePlayFramework &gamePlayFramework) : AGame(gamePlayFramework) {};
        ~Example1Game() override = default;

        void init() override {

        }

        void term() override {

        }

        void update() override {

        }
    private:

};

extern "C" {
    AGame *loadGame(GamePlayFramework &gamePlayFramework) {
        return (new Example1Game(gamePlayFramework));
    }

    void unloadGame(AGame *game) {
        delete game;
    }
}