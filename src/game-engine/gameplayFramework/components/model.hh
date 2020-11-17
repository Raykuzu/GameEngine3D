#pragma once

#include "component.hh"

enum Model : unsigned int {
    MODEL_BOX,
    MODEL_SPHERE
};

typedef struct modelData_s {
    modelData_s() = default;
    virtual ~modelData_s() = default;

    virtual void assign(struct modelData_s *other) = 0;

} modelData_t;

typedef struct model_s : public component_t {
    model_s(int index,Model type = MODEL_BOX, modelData_t *modelData = nullptr): component_t(Component::MODEL), _type(type), _modelData(modelData) {};
    
    Model _type = MODEL_BOX;
    bool _created = false;
    int _index;
    modelData_t *_modelData;

    static component_p createComponent();
    void assign(struct component_s *other) {
        struct model_s *casted = dynamic_cast<struct model_s *>(other);
        _type = casted->_type;
        _created = false;
        _index = casted->_index;
        _modelData = casted->_modelData;
        _modelData->assign(casted->_modelData);
    };
} model_t;



typedef struct boxModel_s: modelData_s {
    boxModel_s(std::vector<EngineMath::Vector3> vect) {model = vect;}
    std::vector<EngineMath::Vector3> model = {
        {0.0,0.0,0.0},
        {0.0,0.0,0.0},
        {0.0,0.0,0.0},
        {0.0,0.0,0.0},
        {0.0,0.0,0.0},
        {0.0,0.0,0.0},
        {0.0,0.0,0.0},
        {0.0,0.0,0.0}
    };
    void assign(struct modelData_s *other) override {
        struct boxModel_s *casted = dynamic_cast<boxModel_s *>(other);
        model = casted->model;
    }
} boxModel_t;

typedef struct sphereModel_s: modelData_s {
    sphereModel_s(std::vector<EngineMath::Vector3> vect) {model = vect;}
    std::vector<EngineMath::Vector3> model = {
        {0.0,0.0,0.0}
    };
    float radius;
    void assign(struct modelData_s *other) override {
        struct sphereModel_s *casted = dynamic_cast<sphereModel_s *>(other);
        model = casted->model;
        radius = casted->radius;
    }
} sphereModel_t;

typedef model_t * model_comp;

component_p model_t::createComponent() {
    return new model_t(0);
};