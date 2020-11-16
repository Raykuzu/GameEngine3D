//
// Created by raykuzu on 06/10/2020.
//

#pragma once

enum Component : unsigned int {
    NO_FLAGS    = 0x00,
    TRANSFORM   = 0x01,
    COLLIDER   = 0x02,
    CAMERA      = 0x04,
    MODEL      = 0x08,
    /*EX1         = 0x02,
    EX2         = 0x04,
    EX3         = 0x08,
    EX4         = 0x16*/
};


typedef struct component_s {
    Component _type;

    explicit component_s(Component type) : _type(type) {};
    virtual ~component_s() = default;

    virtual void assign(struct component_s *other) = 0;

} component_t;

typedef component_t * component_p;
