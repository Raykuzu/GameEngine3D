#ifndef SPHERE_COLLIDER_
#define SPHERE_COLLIDER_

#include <iostream>

#include "ICollider.hpp"

// class SphereCollider: public ICollider {
// public:
//     SphereCollider(EngineMath::Vector3 center, float radius): _center(center), _radius(radius) {};
//     ~SphereCollider() {};
//     Intersection intersect(const SphereCollider &other) const override {
//         float radiuses = _radius + other.getRadius();
//         float distance = _center.distance(other.getCenter());
//         return {radiuses > distance, distance};
//     };
//     Intersection intersect(const AABBCollider &other) const override {
//         return {false, 1};
//     }
//     float getRadius() const {return _radius;};
//     const EngineMath::Vector3 &getCenter() const {return _center;};
// private:
//     EngineMath::Vector3 _center;
//     float _radius;
    
// };

#endif