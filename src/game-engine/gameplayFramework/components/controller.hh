#pragma once

#include "component.hh"
#include "EngineMath.hpp"

typedef struct controller_s : public component_t {
    explicit controller_s() : component_s(Component::CONTROLLER), _shifting({0, 0, 0}) {};

    void assign(component_p other) override {
        struct controller_s *casted = dynamic_cast<struct controller_s *>(other);

        this->_shifting = casted->_shifting;
    }

    static component_p createComponent();

    EngineMath::Vector3 _shifting;

} controller_t;

typedef controller_t * controller_comp_p;

component_p controller_t::createComponent() {
    return new controller_t();
}