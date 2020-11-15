//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include <math.h>
#include "component.hh"
#include "EngineMath.hpp"

#define GRAVITY_CONST EngineMath::Vector3()(0.0f, -9.82f, 0.0f)

enum Collider : unsigned int {
    SPHERE   = 0x01,
    AABB   = 0x02,
    CAPSULE = 0x04,
    OBB = 0x08,
    RAY = 0x16,
    NOCOLLIDER = 0x32,
    /*EX1         = 0x02,
    EX2         = 0x04,
    EX3         = 0x08,
    EX4         = 0x16*/
};

typedef struct colliderData_s {
    colliderData_s() = default;
    virtual ~colliderData_s() = default;

    virtual void assign(struct colliderData_s *other) = 0;

} colliderData_t;

typedef struct collider_s : public component_t {
    Collider _colliderType;
    colliderData_t *_colliderData = nullptr;
    EngineMath::Vector3 _position;
    EngineMath::Vector3 _velocity;
    EngineMath::Vector3 _forces;
    float _mass;
    float _cor;
    float _friction;

    explicit collider_s(Collider type = NOCOLLIDER, colliderData_t *colliderData = nullptr) :
    component_s(Component::COLLIDER), _colliderData(colliderData),  _colliderType(type), _mass(1.0f), _cor(0.5f), _friction(0.6f) {};

    ~collider_s() override {
        if (_colliderData != nullptr) {
            delete _colliderData;
        }
    }
    static component_p createComponent();

    void addCollider(Collider type);

    void assign(struct component_s *other) override {
        struct collider_s *casted = dynamic_cast<collider_s *>(other);

        this->_colliderType = casted->_colliderType;
        this->_position = casted->_position;
        this->_velocity = casted->_velocity;
        this->_forces = casted->_forces;
        this->_mass = casted->_mass;
        this->_cor = casted->_cor;
        this->_friction = casted->_friction;
        if (this->_colliderData != nullptr) {
            this->_colliderData->assign(casted->_colliderData);
        }
    };

} collider_t;

typedef struct AABBCollider_s : public colliderData_t {
    explicit AABBCollider_s(EngineMath::Vector3 const &position = EngineMath::Vector3(), EngineMath::Vector3 const &size = EngineMath::Vector3()):
    colliderData_t(), _position(position), _size(size) {};

    void assign(struct colliderData_s *other) override {
        struct AABBCollider_s *casted = dynamic_cast<AABBCollider_s *>(other);

        this->_position = casted->_position;
        this->_size = casted->_size;
    };

    EngineMath::Vector3 _position;
    EngineMath::Vector3 _size;
} AABBCollider_t;

typedef struct sphereCollider_s : public colliderData_t {
    explicit sphereCollider_s(EngineMath::Vector3 const &position = EngineMath::Vector3(), float radius = 0):
    colliderData_t(), _position(position), _radius(radius) {};

    void assign(struct colliderData_s *other) override {
        struct sphereCollider_s *casted = dynamic_cast<sphereCollider_s *>(other);

        this->_position = casted->_position;
        this->_radius = casted->_radius;
    };


    EngineMath::Vector3 _position;
    float _radius;
} sphereCollider_t;

typedef struct OBBCollider_s : public colliderData_t {
    explicit OBBCollider_s(
        EngineMath::Vector3 const &position = EngineMath::Vector3(),
        EngineMath::Vector3 const &size = EngineMath::Vector3(),
        EngineMath::m3_t const &orientation = EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1)):
    colliderData_t(), _position(position), _size(size), _orientation(orientation) {};

    void assign(struct colliderData_s *other) override {
        struct OBBCollider_s *casted = dynamic_cast<OBBCollider_s *>(other);

        this->_position = casted->_position;
        this->_size = casted->_size;
        this->_orientation = casted->_orientation;
    };

    EngineMath::Vector3 _position;
    EngineMath::Vector3 _size;
    EngineMath::m3_t _orientation;
} OBBCollider_t;

typedef struct rayCollider_s : public colliderData_t {
    explicit rayCollider_s(
        EngineMath::Vector3 const &origin = EngineMath::Vector3(),
        EngineMath::Vector3 const &direction = EngineMath::Vector3()):
    colliderData_t(), _origin(origin), _direction(direction) {
        _direction.normalize();
    };

    void assign(struct colliderData_s *other) override {
        struct rayCollider_s *casted = dynamic_cast<rayCollider_s *>(other);

        this->_origin = casted->_origin;
        this->_direction = casted->_direction;
    };

    EngineMath::Vector3 _origin;
    EngineMath::Vector3 _direction;
} rayCollider_t;

typedef struct capsuleCollider_s : public colliderData_t {
    capsuleCollider_s(EngineMath::Vector3 tip = EngineMath::Vector3(), EngineMath::Vector3 base = EngineMath::Vector3(), float radius = 0):
    colliderData_t(), _tip(tip), _base(base), _radius(radius) {};
    
    EngineMath::Vector3 _tip;
    EngineMath::Vector3 _base;
    float _radius;
    void assign(struct colliderData_s *other) override {
        struct capsuleCollider_s *casted = dynamic_cast<capsuleCollider_s *>(other);

        this->_tip = casted->_tip;
        this->_base = casted->_base;
        this->_radius = casted->_radius;
    };
} capsuleCollider_t;

typedef collider_t * collider_comp;

component_p collider_t::createComponent() {
    return new collider_t();
}