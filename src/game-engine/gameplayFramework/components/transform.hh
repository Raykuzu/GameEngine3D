//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "component.hh"
#include "EngineMath.hpp"

typedef struct transform_s : public component_t {
    transform_s(EngineMath::Vector3 position, EngineMath::Vector3 velocity = EngineMath::Vector3(), EngineMath::Vector3 acceleration = EngineMath::Vector3())
    : component_s(Component::TRANSFORM), _position(position), _velocity(velocity), _acceleration(acceleration) {};

    EngineMath::Vector3 _position;
    EngineMath::Vector3 _velocity;
    EngineMath::Vector3 _acceleration;
    static component_p createComponent();
    // TODO A vous de vous démerder pour les datas
} transform_t;

typedef transform_t * transform_comp_p;

component_p transform_t::createComponent() {
    return new transform_t(EngineMath::Vector3());
}