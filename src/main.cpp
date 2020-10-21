//
// Created by raykuzu on 30/09/2020.
//
#include <iostream>
#include "collider.hh"
#include "PhysicsModule.hpp"
#include "ArcLogger.hpp"

int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av) {
    sphereCollider_t sp1(EngineMath::Vector3(0.0f,0.0f,0.0f), 5.0f);
    sphereCollider_t sp2(EngineMath::Vector3(1.0f,1.0f,1.0f), 5.0f);
    sphereCollider_t sp3(EngineMath::Vector3(-1.0f,-1.0f,-1.0f), 5.0f);

    AABBCollider_t aa1(EngineMath::Vector3(0.0f,0.0f,0.0f), EngineMath::Vector3(1.0f,1.0f,1.0f));
    AABBCollider_t aa2(EngineMath::Vector3(1.0f,1.0f,1.0f), EngineMath::Vector3(2.0f,2.0f,2.0f));
    AABBCollider_t aa3(EngineMath::Vector3(1.0f,0.0f,0.0f), EngineMath::Vector3(2.0f,1.0f,1.0f));
    AABBCollider_t aa4(EngineMath::Vector3(0.0f,0.0f,-2.0f), EngineMath::Vector3(1.0f,1.0f,-1.0f));
    AABBCollider_t aa5(EngineMath::Vector3(0.0f,0.5f,0.0f), EngineMath::Vector3(1.0f,1.5f,1.0f));

    Intersection inter1 = sp2.intersect(sp1);
    Intersection inter2 = sp1.intersect(sp3);

    Intersection inter3 = aa1.intersect(aa2);
    Intersection inter4 = aa1.intersect(aa3);
    Intersection inter5 = aa1.intersect(aa4);
    Intersection inter6 = aa1.intersect(aa5);

    std::cout << inter1.collided << " dist==" << inter1.distance << std::endl;
    std::cout << inter2.collided << " dist==" << inter2.distance << std::endl;
    std::cout << inter3.collided << " dist==" << inter3.distance << std::endl;
    std::cout << inter4.collided << " dist==" << inter4.distance << std::endl;
    std::cout << inter5.collided << " dist==" << inter5.distance << std::endl;
    std::cout << inter6.collided << " dist==" << inter6.distance << std::endl;

    return (0);
}