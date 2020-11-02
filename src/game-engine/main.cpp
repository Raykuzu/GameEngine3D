//
// Created by raykuzu on 30/09/2020.
//

#include "PhysicsModule.hpp"
#include "Shell.hpp"

int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av) {
    ArcLogger::setBinaryName("GE3D");
    ArcLogger::setOutputDir("./log");
    ArcLogger::setLogLevel(ArcLogger::TRACE);
    ArcLogger::setLogLevel(ArcLogger::DEBUG);
    Shell shell;

    std::vector<GameObject *> list;
    GameObject *go = new GameObject(0x01 | 0x02);
    static_cast<collider_t*>(go->getComponent(Component::COLLIDER))->_colliderType = Collider::SPHERE;
    static_cast<collider_t*>(go->getComponent(Component::COLLIDER))->_colliderData = new sphereCollider_t(EngineMath::Vector3(0,0,0), 1.1);
    static_cast<transform_t*>(go->getComponent(Component::TRANSFORM))->_velocity = EngineMath::Vector3(1,0,0);
    list.push_back(go);

    go = new GameObject(0x01 | 0x02);
    static_cast<collider_t*>(go->getComponent(Component::COLLIDER))->_colliderType = Collider::SPHERE;
    static_cast<collider_t*>(go->getComponent(Component::COLLIDER))->_colliderData = new sphereCollider_t(EngineMath::Vector3(4,0,0), 1.1);
    static_cast<transform_t*>(go->getComponent(Component::TRANSFORM))->_velocity = EngineMath::Vector3(0,0,0);
    static_cast<transform_t*>(go->getComponent(Component::TRANSFORM))->_position = EngineMath::Vector3(4,0,0);

    list.push_back(go);

    
    PhysicsModule phm = PhysicsModule(list);

    for (int i = 0; i < 5; i++) {
        phm.update();
        phm.display();
    }

    phm.testModule();

    // shell.run();
    return (0);
}