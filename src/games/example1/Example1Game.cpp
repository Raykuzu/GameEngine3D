//
// Created by raykuzu on 19/10/2020.
//

#include "AGame.hpp"
#include "InputTrigger.hpp"
#include "CollideTrigger.hpp"

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
            _gamePlayFramework.addTriggerToScene("scene1", new InputTrigger(WI_L, [](sharedGO self) {}));

            sharedGO object1 = _gamePlayFramework.createGameObject("scene1", Component::TRANSFORM);
            sharedGO object2 = _gamePlayFramework.createGameObject("scene1", Component::TRANSFORM);
            object1->addTrigger(new InputTrigger(WI_0, [](sharedGO &self) {
                //self->coupdépée
            }));
            object1->addTrigger(new InputTrigger(WI_LEFT, [](sharedGO &self) {
                self->getComponent<transform_comp_p>(TRANSFORM)->_position.x -= 50;
            }));

            //exemple collision sans condition d'object
            object1->addTrigger(new CollideTrigger([](sharedGO &self, sharedGO &other) {
                //trigger func
                // self.healthComponent.health -= other.healthComponent.bodyDamages; <- exemple
            }));
            std::function<void (sharedGO &, sharedGO &)> func;
            std::function<bool (sharedGO const &, sharedGO const &)> func1;

            //exemple avec condition
            object1->addTrigger(new CollideTrigger([](sharedGO const &self, sharedGO const &other) -> bool {
                // condition func
                // if other.healthComp.sometype == sometype <- exemple
                //      return true
                //return false
                return (true);
                },[](sharedGO &, sharedGO &) {
                // trigger func
            }));
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