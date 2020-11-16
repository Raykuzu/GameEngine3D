#pragma once

#include "component.hh"
#include "Camera.hpp"

typedef struct camera_s : public component_t {
    explicit camera_s(): component_s(Component::CAMERA)  {
        camera = new OrbitCamera();
    };

    OrbitCamera *camera;
    void assign(struct component_s *other) {
        struct camera_s *casted = dynamic_cast<struct camera_s *>(other);

        camera = casted->camera;
    };
    static component_p createComponent();

} camera_t;

typedef camera_t * camera_comp;

component_p camera_t::createComponent() {
    return new camera_t();
}