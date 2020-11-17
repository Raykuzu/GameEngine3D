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

            _object1 = _gamePlayFramework.createGameObject("scene1", Component::RIGID_BODY | Component::CAMERA | Component::TRANSFORM | Component::CONTROLLER | Component::MODEL);
            _object2 = _gamePlayFramework.createGameObject("scene1", Component::RIGID_BODY | Component::TRANSFORM | Component::MODEL);

            boxModel_t *cub = new boxModel_t({
                {0.5f, 0.5f - 4, 0.5f},
                {1.5f, 0.5f - 4, 0.5f},
                {0.5f, 1.5f - 4, 0.5f},
                {1.5f, 1.5f - 4, 0.5f},

                {0.5f, 0.5f - 4, 1.5f},
                {1.5f, 0.5f - 4, 1.5f},
                {0.5f, 1.5f - 4, 1.5f},
                {1.5f, 1.5f - 4, 1.5f}
                
            });

            boxModel_t *cub2 = new boxModel_t({
                {2.5f, 0.5f - 4, 2.5f},
                {3.5f, 0.5f - 4, 2.5f},
                {2.5f, 1.5f - 4, 2.5f},
                {3.5f, 1.5f - 4, 2.5f},

                {2.5f, 0.5f - 4, 3.5f},
                {3.5f, 0.5f - 4, 3.5f},
                {2.5f, 1.5f - 4, 3.5f},
                {3.5f, 1.5f - 4, 3.5f}
                
            });

            model_t *mdl= new model_t(1, MODEL_BOX, cub);
            model_t *mdl2= new model_t(2, MODEL_BOX, cub2);

            OBBCollider_t *obb1 = new OBBCollider_t (EngineMath::Vector3(1.0f, -3.0f, 1.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));
            rigidBody_comp_p object1Collider = new rigidBody_t(OBB, obb1);
            transform_comp_p object1Transform = new transform_t(EngineMath::Vector3(1.0f, -3.0f, 1.0f), EngineMath::Vector3(0.0f, 0.0f, 0.0f), EngineMath::Vector3(0.0f, 0.0f, 0.0f));

            OBBCollider_t *obb2 = new OBBCollider_t (EngineMath::Vector3(3.0f, -3.0f, 3.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));
            rigidBody_comp_p object2Collider = new rigidBody_t(OBB, obb2);
            transform_comp_p object2Transform = new transform_t(EngineMath::Vector3(3.0f, -3.0f, 3.0f), EngineMath::Vector3(0.0f, 0.0f, 0.0f), EngineMath::Vector3(0.0f, 0.0f, 0.0f));

            OBBCollider_t *constraintObb = new OBBCollider_t (EngineMath::Vector3(0.0f, .750f, 0.0f), EngineMath::Vector3(100.0f, 1.0f, 100.0f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));
            rigidBody_comp_p constraintCollider = new rigidBody_t(OBB, constraintObb);

            _gamePlayFramework.addConstraintToScene("scene1", constraintCollider);

            _object1->setComponent<model_t *>(Component::MODEL, mdl);
            _object1->setComponent<rigidBody_comp_p>(Component::RIGID_BODY, object1Collider);
            _object1->setComponent<transform_comp_p>(Component::TRANSFORM, object1Transform);

            _object2->setComponent<model_t *>(Component::MODEL, mdl2);
            _object2->setComponent<rigidBody_comp_p>(Component::RIGID_BODY, object2Collider);
            _object2->setComponent<transform_comp_p>(Component::TRANSFORM, object2Transform);

            for (auto i : reinterpret_cast<boxModel_t *>(_object1->getComponent<model_t *>(Component::MODEL)->_modelData)->model) {
                std::cout << i.x << " "<< i.y << " "<< i.z;
            }

            InputTrigger *trig = new InputTrigger(WI_Q, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Pan(EngineMath::Vector3(0.03,0.0, 0.0), 1);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);
            
            trig = new InputTrigger(WI_O, [](sharedGO &self) {
                self->getComponent<controller_comp_p>(Component::CONTROLLER)->_shifting.z += 3;
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_L, [](sharedGO &self) {
                self->getComponent<controller_comp_p>(Component::CONTROLLER)->_shifting.z -= 3;
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_K, [](sharedGO &self) {
                self->getComponent<controller_comp_p>(Component::CONTROLLER)->_shifting.x -= 3;
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_M, [](sharedGO &self) {
                self->getComponent<controller_comp_p>(Component::CONTROLLER)->_shifting.x += 3;
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_D, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Pan(EngineMath::Vector3(-0.03,0.0, 0.0), 1);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_Z, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Pan(EngineMath::Vector3(0.0,0.0, -0.03), 1);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);
            trig = new InputTrigger(WI_S, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Pan(EngineMath::Vector3(0.0,0.0, 0.03), 1);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

                        trig = new InputTrigger(WI_SPACE, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Pan(EngineMath::Vector3(0.0,0.03, 0.0), 1);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);
            trig = new InputTrigger(WI_X, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Pan(EngineMath::Vector3(0.0,-0.03, 0.00), 1);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_RIGHT, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Rotate(EngineMath::Vector2(0.1,0), 1.0);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_LEFT, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Rotate(EngineMath::Vector2(-0.1,0), 1.0);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);


            trig = new InputTrigger(WI_UP, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Rotate(EngineMath::Vector2(0,0.1), 1.0);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);

            trig = new InputTrigger(WI_DOWN, [](sharedGO &self) {
                self->getComponent<camera_comp>(CAMERA)->camera->Rotate(EngineMath::Vector2(0,-0.1), 1.0);
            });
            trig->setSubject(_object1);
            _object1->addTrigger(trig);
        }

        void term() override {
            ArcLogger::trace("Example1Game::term");
        }

        void update() override {
            ArcLogger::trace("Example1Game::update");
        }

    private:
        sharedGO _object1;
        sharedGO _object2;
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