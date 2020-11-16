#pragma once

#include "component.hh"
#include "GraphicModule.hpp"

typedef struct model_s : public component_t {
    model_s(int index, modelData_t *modelData = nullptr): component_t(Component::MODEL), _modelData(modelData) {};
    bool _created = false;
    int _index;
    modelData_t *_modelData;

    static component_p createComponent();
    void assign(struct component_s *other) {
        struct model_s *casted = dynamic_cast<struct model_s *>(other);
        bool _created = false;
        _index = casted->_index;
        _modelData = casted->_modelData;

    };
} model_t;

typedef struct modelData_s {
    modelData_s() = default;
    virtual ~modelData_s() = default;

    virtual void assign(struct modelData_s *other) = 0;

} modelData_t;

typedef struct boxModel: modelData_s {
    std::vector<Vertex> model;
} modelData_t;

typedef model_t * model_comp;

component_p model_t::createComponent() {
    return new model_t(0);
}