//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "component.hh"
#include "EngineMath.hpp"

enum Collider : unsigned int {
    SPHERE   = 0x01,
    AABB   = 0x02,
    NOCOLLIDER = 0x4,
    /*EX1         = 0x02,
    EX2         = 0x04,
    EX3         = 0x08,
    EX4         = 0x16*/
};

typedef struct colliderData_s {
    colliderData_s() {};

} colliderData_t;

typedef struct collider_s : public component_t {
    Collider _colliderType;
    colliderData_t *_colliderData;

    collider_s(Collider type = NOCOLLIDER) : component_s(Component::COLLIDER), _colliderType(type) {};
    static component_p createComponent();

    // TODO A vous de vous démerder pour les datas
} collider_t;

typedef struct AABBCollider_s : public colliderData_t {
    AABBCollider_s(EngineMath::Vector3 minExtend = EngineMath::Vector3(), EngineMath::Vector3 maxExtend = EngineMath::Vector3()):
    colliderData_t(), _minExtend(minExtend), _maxExtend(maxExtend) {};

    EngineMath::Vector3 _minExtend;
    EngineMath::Vector3 _maxExtend;
    // TODO A vous de vous démerder pour les datas
} AABBCollider_t;

typedef struct sphereCollider_s : public colliderData_t {
    sphereCollider_s(EngineMath::Vector3 center = EngineMath::Vector3(), float radius = 0):
    colliderData_t(), _center(center), _radius(radius) {};
    
    EngineMath::Vector3 _center;
    float _radius;
    // TODO A vous de vous démerder pour les datas
} sphereCollider_t;

typedef collider_t * collider_comp;

component_p collider_t::createComponent() {
    return new collider_t();
}