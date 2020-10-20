//
// Created by raykuzu on 19/10/2020.
//

#include "IGame.hpp"

class Example1Game : public IGame {
    public:
        Example1Game() = default;
        ~Example1Game() = default;

        virtual void init(GamePlayFramework gamePlayFramework) {

        }

        virtual void term(GamePlayFramework gamePlayFramework) {

        }

        virtual void update(GamePlayFramework gamePlayFramework) {

        }
    private:

};

extern "C" {
    Example1Game *loadGame() {
        return (new Example1Game());
    }

    void unloadGame(Example1Game *example1Game) {
        delete example1Game;
    }
}