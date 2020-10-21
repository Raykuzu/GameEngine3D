//
// Created by raykuzu on 30/09/2020.
//
#include <iostream>

#include "Core.hpp"
#include "PhysicsModule.hpp"
int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av) {
    ArcLogger::setBinaryName("GE3D");
    ArcLogger::setOutputDir("./log");
    ArcLogger::setLogLevel(ArcLogger::TRACE);
    ArcLogger::setLogLevel(ArcLogger::DEBUG);
    Core core = Core();

    std::vector<GameObject *> list;

    PhysicsModule phm = PhysicsModule(list);

    transform_t tr(EngineMath::Vector3(), EngineMath::Vector3(1,1,1));

    sphereCollider_t *sp1 = new sphereCollider_t(EngineMath::Vector3(0.0f,0.0f,0.0f), 5.0f);
    sphereCollider_t *sp2 = new sphereCollider_t(EngineMath::Vector3(1.0f,1.0f,1.0f), 5.0f);
    sphereCollider_t *sp3 = new sphereCollider_t(EngineMath::Vector3(-1.0f,-1.0f,-1.0f), 5.0f);

    // collider_t * coll = new collider 

    AABBCollider_t *aa1 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.0f,0.0f), EngineMath::Vector3(1.0f,1.0f,1.0f));
    AABBCollider_t *aa2 = new AABBCollider_t (EngineMath::Vector3(1.0f,1.0f,1.0f), EngineMath::Vector3(2.0f,2.0f,2.0f));
    AABBCollider_t *aa3 = new AABBCollider_t (EngineMath::Vector3(1.0f,0.0f,0.0f), EngineMath::Vector3(2.0f,1.0f,1.0f));
    AABBCollider_t *aa4 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.0f,-2.0f), EngineMath::Vector3(1.0f,1.0f,-1.0f));
    AABBCollider_t *aa5 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.5f,0.0f), EngineMath::Vector3(1.0f,1.5f,1.0f));

    Intersection inter1 = phm.intersect(sp2, sp1);
    Intersection inter2 = phm.intersect(sp1, sp3);

    Intersection inter3 = phm.intersect(aa1, aa2);
    Intersection inter4 = phm.intersect(aa1, aa3);
    Intersection inter5 = phm.intersect(aa1, aa4);
    Intersection inter6 = phm.intersect(aa1, aa5);

    std::cout << inter1.collided << " dist==" << inter1.distance << std::endl;
    std::cout << inter2.collided << " dist==" << inter2.distance << std::endl;
    std::cout << inter3.collided << " dist==" << inter3.distance << std::endl;
    std::cout << inter4.collided << " dist==" << inter4.distance << std::endl;
    std::cout << inter5.collided << " dist==" << inter5.distance << std::endl;
    std::cout << inter6.collided << " dist==" << inter6.distance << std::endl;

    phm.integrate(tr, 1);
    phm.diplay(tr);
    phm.integrate(tr, 1);
    phm.diplay(tr);    phm.integrate(tr, 1);
    phm.diplay(tr);    phm.integrate(tr, 1);
    phm.diplay(tr);
    core.run();
    return (0);
}