#ifndef TRANSFORM_COMPONENT_
#define TRANSFORM_COMPONENT_

#include "transform.hh"
#include "EngineMath.hpp"

class TransformComponent: public transform_s {
public:
    TransformComponent(EngineMath::Vector3 position, EngineMath::Vector3 velocity = EngineMath::Vector3(), EngineMath::Vector3 acceleration = EngineMath::Vector3()):
        _position(position),
        _velocity(velocity),
        _acceleration(acceleration)
        {}; 
    ~TransformComponent() {};

    void integrate(float delta) {
        _position += _velocity * delta;
    }
private:
    EngineMath::Vector3 _position;
    EngineMath::Vector3 _velocity;
    EngineMath::Vector3 _acceleration;
};

#endif