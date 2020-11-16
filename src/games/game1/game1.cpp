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
            ArcLogger::trace("game1::init");

            _gamePlayFramework.createScene("scene1");

            _object1 = _gamePlayFramework.createGameObject("scene1", Component::COLLIDER);

            OBBCollider_t *obb1 = new OBBCollider_t (EngineMath::Vector3(0.0f, 1.0f, 0.0f), EngineMath::Vector3(0.5f, 1.0f, 0.5f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));
            collider_comp object1Collider = new collider_t(OBB, obb1);

            _object1->setComponent<collider_comp>(COLLIDER, object1Collider);

            _object1->addTrigger(new InputTrigger(WI_Q, [](sharedGO &self) {
                self->getComponent<collider_comp>(COLLIDER)->_position.x -= 50;
            }));
            _object1->addTrigger(new InputTrigger(WI_D, [](sharedGO &self) {
                self->getComponent<collider_comp>(COLLIDER)->_position.x += 50;
            }));
            _object1->addTrigger(new InputTrigger(WI_Z, [](sharedGO &self) {
                self->getComponent<collider_comp>(COLLIDER)->_position.z += 50;
            }));
            _object1->addTrigger(new InputTrigger(WI_S, [](sharedGO &self) {
                self->getComponent<collider_comp>(COLLIDER)->_position.z -= 50;
            }));
        }

        void term() override {
            ArcLogger::trace("Example1Game::term");

        }

        void update() override {
            ArcLogger::trace("Example1Game::update");

        }
    private:
        sharedGO _object1;
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