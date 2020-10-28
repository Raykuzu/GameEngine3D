//
// Created by raykuzu on 06/10/2020.
//

#pragma once

#include "component.hh"

typedef struct transform_s : public component_t {
    transform_s() : component_s(Component::TRANSFORM) {};

    void assign(component_p other) override {
        struct transform_s *casted = dynamic_cast<struct transform_s *>(other);

        this->test = casted->test;
    }

    static component_p createComponent();

    int test = 0;
    // TODO A vous de vous démerder pour les datas
} transform_t;

typedef transform_t * transform_comp_p;

component_p transform_t::createComponent() {
    return new transform_t();
}