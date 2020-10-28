//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "component.hh"
#include "EngineMath.hpp"

typedef struct transform_s : public component_t {
    explicit transform_s(EngineMath::Vector3 const &position, EngineMath::Vector3 const &velocity = EngineMath::Vector3(), EngineMath::Vector3 const &acceleration = EngineMath::Vector3())
    : component_s(Component::TRANSFORM), _position(position), _velocity(velocity), _acceleration(acceleration) {};

    void assign(component_p other) override {
        struct transform_s *casted = dynamic_cast<struct transform_s *>(other);

        this->_position = casted->_position;
        this->_velocity = casted->_velocity;
        this->_acceleration = casted->_acceleration;
    }

    static component_p createComponent();

    EngineMath::Vector3 _position;
    EngineMath::Vector3 _velocity;
    EngineMath::Vector3 _acceleration;

} transform_t;

typedef transform_t * transform_comp_p;

component_p transform_t::createComponent() {
    return new transform_t(EngineMath::Vector3());
}