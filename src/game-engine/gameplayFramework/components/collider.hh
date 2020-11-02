//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "component.hh"
#include "EngineMath.hpp"

enum Collider : unsigned int {
    SPHERE   = 0x01,
    AABB   = 0x02,
    NOCOLLIDER = 0x04,
    /*EX1         = 0x02,
    EX2         = 0x04,
    EX3         = 0x08,
    EX4         = 0x16*/
};

typedef struct colliderData_s {
    colliderData_s() = default;
    ~colliderData_s() = default;

    virtual void assign(struct colliderData_s *other) = 0;

} colliderData_t;

typedef struct collider_s : public component_t {
    Collider _colliderType;
    colliderData_t *_colliderData = nullptr;

    explicit collider_s(Collider type = NOCOLLIDER) : component_s(Component::COLLIDER), _colliderType(type) {};
    static component_p createComponent();

    void addCollider(Collider type);

    void assign(struct component_s *other) override {
        struct collider_s *casted = dynamic_cast<collider_s *>(other);

        this->_colliderType = casted->_colliderType;
        if (this->_colliderData != nullptr) {
            this->_colliderData->assign(casted->_colliderData);
        }
    };


    // TODO A vous de vous démerder pour les datas
} collider_t;

typedef struct AABBCollider_s : public colliderData_t {
    explicit AABBCollider_s(EngineMath::Vector3 const &minExtend = EngineMath::Vector3(), EngineMath::Vector3 const &maxExtend = EngineMath::Vector3()):
    colliderData_t(), _minExtend(minExtend), _maxExtend(maxExtend) {};

    void assign(struct colliderData_s *other) override {
        struct AABBCollider_s *casted = dynamic_cast<AABBCollider_s *>(other);

        this->_minExtend = casted->_minExtend;
        this->_maxExtend = casted->_maxExtend;
    };


    EngineMath::Vector3 _minExtend;
    EngineMath::Vector3 _maxExtend;
    // TODO A vous de vous démerder pour les datas
} AABBCollider_t;

typedef struct sphereCollider_s : public colliderData_t {
    explicit sphereCollider_s(EngineMath::Vector3 const &center = EngineMath::Vector3(), float radius = 0):
    colliderData_t(), _center(center), _radius(radius) {};

    void assign(struct colliderData_s *other) override {
        struct sphereCollider_s *casted = dynamic_cast<sphereCollider_s *>(other);

        this->_center = casted->_center;
        this->_radius = casted->_radius;
    };


    EngineMath::Vector3 _center;
    float _radius;
    // TODO A vous de vous démerder pour les datas
} sphereCollider_t;

void collider_s::addCollider(Collider type) {
        _colliderType = type;
        switch (type) {
            case SPHERE:
                _colliderData = new sphereCollider_t ();
                break;
            case AABB:
                _colliderData = new AABBCollider_t ();
                break;
            case NOCOLLIDER:
                break;
        }
}

typedef collider_t * collider_comp;

component_p collider_t::createComponent() {
    return new collider_t();
}