//
// Created by raykuzu on 06/10/2020.
//

#pragma once

enum Component : unsigned int {
    TRANSFORM   = 0x01,
    COLLIDER   = 0x02,
    /*EX1         = 0x02,
    EX2         = 0x04,
    EX3         = 0x08,
    EX4         = 0x16*/
};


typedef struct component_s {
    Component _type;

    explicit component_s(Component type) : _type(type) {};
} component_t;

typedef component_t * component_p;
