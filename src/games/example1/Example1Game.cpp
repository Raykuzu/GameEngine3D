//
// Created by raykuzu on 19/10/2020.
//

#include "AGame.hpp"

class Example1Game : public AGame {
    public:
        explicit Example1Game(GamePlayFramework &gamePlayFramework) : AGame(gamePlayFramework) {
            ArcLogger::setBinaryName("GE3D-GameExample1");
            ArcLogger::setOutputDir("./log");
            ArcLogger::setLogLevel(ArcLogger::TRACE);
            ArcLogger::setLogLevel(ArcLogger::DEBUG);
        };
        ~Example1Game() override = default;

        void init() override {
            ArcLogger::trace("Example1Game::init");
            _gamePlayFramework.createScene("scene1");
            sharedGO object1 = _gamePlayFramework.createGameObject("scene1", Component::TRANSFORM);
            sharedGO object2 = _gamePlayFramework.createGameObject("scene1", Component::TRANSFORM);

        }

        void term() override {
            ArcLogger::trace("Example1Game::term");

        }

        void update() override {
            ArcLogger::trace("Example1Game::update");

        }
    private:

};

extern "C" {
    AGame *loadGame(GamePlayFramework &gamePlayFramework) {
        ArcLogger::trace("Extern loadGame");
        return (new Example1Game(gamePlayFramework));
    }

    void unloadGame(AGame *game) {
        ArcLogger::trace("Extern unloadGame");
        delete game;
    }
}