//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include <math.h>
#include "component.hh"
#include "EngineMath.hpp"

enum Collider : unsigned int {
    SPHERE   = 0x01,
    AABB   = 0x02,
    CAPSULE = 0x04,
    OBB = 0x08,
    NOCOLLIDER = 0x16,
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

    explicit collider_s(Collider type = NOCOLLIDER, colliderData_t *colliderData = nullptr) : component_s(Component::COLLIDER), _colliderData(colliderData),  _colliderType(type) {};
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
    explicit AABBCollider_s(EngineMath::Vector3 const &position = EngineMath::Vector3(), EngineMath::Vector3 const &size = EngineMath::Vector3()):
    colliderData_t(), _position(position), _size(size) {};

    void assign(struct colliderData_s *other) override {
        struct AABBCollider_s *casted = dynamic_cast<AABBCollider_s *>(other);

        this->_position = casted->_position;
        this->_size = casted->_size;
    };

    EngineMath::Vector3 getClosestPoint(EngineMath::Vector3 const &point) const {
        EngineMath::Vector3 min = getMin();
        EngineMath::Vector3 max = getMax();

        return {
            fminf(fmaxf(point.x, min.x), max.x),
            fminf(fmaxf(point.y, min.y), max.y),
            fminf(fmaxf(point.z, min.z), max.z)
        };
    }

    EngineMath::Vector3 getMin() const {
        EngineMath::Vector3 p1 = _position + _size;
        EngineMath::Vector3 p2 = _position - _size;

        return EngineMath::Vector3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
    }

    EngineMath::Vector3 getMax() const {
        EngineMath::Vector3 p1 = _position + _size;
        EngineMath::Vector3 p2 = _position - _size;

        return EngineMath::Vector3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
    }

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
        EngineMath::m3_t const &orientation = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }):
    colliderData_t(), _position(position), _size(size), _orientation(orientation) {};

    void assign(struct colliderData_s *other) override {
        struct OBBCollider_s *casted = dynamic_cast<OBBCollider_s *>(other);

        this->_position = casted->_position;
        this->_size = casted->_size;
        this->_orientation = casted->_orientation;
    };

    EngineMath::Vector3 getAxis(int index) {
        const float* orientation = _orientation[index];

        return {
            orientation[0],
            orientation[1],
            orientation[2]
        };
    }

    EngineMath::Vector3 getClosestPoint(EngineMath::Vector3 const &point) {
        EngineMath::Vector3 result(point);
        EngineMath::Vector3 direction = point - _position;
        float sizes[3] = { _size.x, _size.y, _size.z };

        for (int i = 0; i != 3; i += 1) {
            EngineMath::Vector3 axis = getAxis(i);
            float distance = direction.dot(axis);

            distance = fmaxf(fminf(distance, sizes[i]), -sizes[i]);
            result += (axis * distance);
        }

        return result;
    }

    EngineMath::Vector3 _position;
    EngineMath::Vector3 _size;
    EngineMath::m3_t _orientation;
} OBBCollider_t;

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

void collider_s::addCollider(Collider type) {
        _colliderType = type;
        switch (type) {
            case SPHERE:
                _colliderData = new sphereCollider_t ();
                break;
            case AABB:
                _colliderData = new AABBCollider_t ();
                break;
            case CAPSULE:
                _colliderData = new capsuleCollider_t ();
                break;
            case NOCOLLIDER:
                break;
        }
}

typedef collider_t * collider_comp;

component_p collider_t::createComponent() {
    return new collider_t();
}