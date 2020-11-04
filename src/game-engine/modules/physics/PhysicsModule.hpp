//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include <assert.h>
#include <utility>
#include <algorithm>
#include "AModule.hpp"
#include "ArcLogger.hpp"
#include "collider.hh"
#include "Intersection.hpp"

#define Square(x) ((x)*(x))
#define ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }
typedef std::pair<EngineMath::Vector3, EngineMath::Vector3> CapsuleCenters;

class PhysicsModule : public AModule {
    public:
        explicit PhysicsModule(std::vector<sharedGO> &gameObjects) : AModule(gameObjects) {};
        ~PhysicsModule() override = default;

        Intersection intersect(const AABBCollider_t *a, const AABBCollider_t *b) const {
            EngineMath::Vector3 minA = a->getMin();
            EngineMath::Vector3 minB = b->getMin();
            EngineMath::Vector3 maxA = a->getMax();
            EngineMath::Vector3 maxB = b->getMax();

            return {
                (minA.x <= maxB.x && maxA.x >= minB.x) &&
                (minA.y <= maxB.y && maxA.y >= minB.y) &&
                (minA.z <= maxB.z && maxA.z >= minB.z)
            , 1};
        };

        Intersection intersect(const sphereCollider_t *a, const sphereCollider_t *b) const {
            float radiuses = a->_radius + b->_radius;
            float distance = a->_position.distance(b->_position);

            return {radiuses > distance, distance - radiuses};
        };

        Intersection intersect(const AABBCollider_t *a, const sphereCollider_t *b) const {
            EngineMath::Vector3 closestPoint = a->getClosestPoint(b->_position);
            float distSquare = (b->_position - closestPoint).dotSquare();
            float radiusSquare = b->_radius * b->_radius;

            return { distSquare < radiusSquare, distSquare - radiusSquare };
        }

        Intersection intersect(const OBBCollider_t *a, const sphereCollider_t *b) const {
            EngineMath::Vector3 closestPoint = a->getClosestPoint(b->_position);
            float distSquare = (b->_position - closestPoint).dotSquare();
            float radiusSquare = b->_radius * b->_radius;

            return { distSquare < radiusSquare, distSquare - radiusSquare };
        }

        // Intersection intersect(const OBBCollider_t *a, const OBBCollider_t *b) const {
        // }

        // Intersection intersect(const AABBCollider_t *a, const sphereCollider_t *b) const {
        //     float squaredRadius = Square(b->_radius);

        //     if (b->_position.x < a->_minExtend.x)
        //         squaredRadius -= Square(b->_position.x - a->_minExtend.x);
        //     else if (b->_position.x > a->_maxExtend.x)
        //         squaredRadius -= Square(b->_position.x - a->_maxExtend.x);

        //     if (b->_position.y < a->_minExtend.y)
        //         squaredRadius -= Square(b->_position.y - a->_minExtend.y);
        //     else if (b->_position.y > a->_maxExtend.y)
        //         squaredRadius -= Square(b->_position.y - a->_maxExtend.y);

        //     if (b->_position.z < a->_minExtend.z)
        //         squaredRadius -= Square(b->_position.z - a->_minExtend.z);
        //     else if (b->_position.z > a->_maxExtend.z)
        //         squaredRadius -= Square(b->_position.z - a->_maxExtend.z);

        //     return { squaredRadius > 0, -squaredRadius };
        // }

        // EngineMath::Vector3 findClosestIntersectionPoint(EngineMath::Vector3 A, EngineMath::Vector3 B, EngineMath::Vector3 Point) const {
        //     EngineMath::Vector3 AB = B - A;
        //     float t = (Point - A).dot(AB) / AB.dot(AB);
        //     return A + AB * std::min(std::max(t, 0.0f), 1.0f);
        // }

        // CapsuleCenters getCapsuleCenters(const capsuleCollider_t * edge) const {
        //     EngineMath::Vector3 normal(edge->_tip - edge->_base);
        //     normal.normalize();
        //     EngineMath::Vector3 aOffset = normal * edge->_radius;

        //     return CapsuleCenters { edge->_base + aOffset, edge->_tip - aOffset };
        // }

        // Intersection intersect(const capsuleCollider_t *a, const capsuleCollider_t *b) const {
        //     CapsuleCenters aCenters = getCapsuleCenters(a);
        //     CapsuleCenters bCenters = getCapsuleCenters(b);

        //     EngineMath::Vector3 v0 = bCenters.first - aCenters.first;
        //     EngineMath::Vector3 v1 = bCenters.second - aCenters.first;
        //     EngineMath::Vector3 v2 = bCenters.first - aCenters.second;
        //     EngineMath::Vector3 v3 = bCenters.second - aCenters.second;

        //     float d0 = v0.dot(v0);
        //     float d1 = v1.dot(v1);
        //     float d2 = v2.dot(v2);
        //     float d3 = v3.dot(v3);

        //     EngineMath::Vector3 bestA = (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) ? aCenters.second : aCenters.first;
        //     EngineMath::Vector3 bestB = findClosestIntersectionPoint(bCenters.first, bCenters.second, bestA);

        //     bestA = findClosestIntersectionPoint(aCenters.first, aCenters.second, bestB);
        //     return intersect(new sphereCollider_t(bestA, a->_radius), new sphereCollider_t(bestB, b->_radius));
        // };

        // float computeHalfSizeOnAxis(const OBBCollider_t *obb, const EngineMath::Vector3 axis) const {
        //     return (fabs((obb->_xAxis * obb->_halfSize.x).dot(axis)) + fabs((obb->_yAxis * obb->_halfSize.y).dot(axis)) + fabs((obb->_zAxis * obb->_halfSize.z).dot(axis)));
        // }


        // Intersection intersect(const OBBCollider_t *a, const OBBCollider_t *b) const {
        //     const EngineMath::Vector3 gLength = b->_center - a->_center;
        //     std::vector<EngineMath::Vector3> axes = {
        //         a->_xAxis, a->_yAxis, a->_zAxis,
        //         b->_xAxis, b->_yAxis, b->_zAxis,
        //         a->_xAxis.cross(b->_xAxis), a->_xAxis.cross(b->_yAxis), a->_xAxis.cross(b->_zAxis),
        //         a->_yAxis.cross(b->_xAxis), a->_yAxis.cross(b->_yAxis), a->_yAxis.cross(b->_zAxis),
        //         a->_zAxis.cross(b->_xAxis), a->_zAxis.cross(b->_yAxis), a->_zAxis.cross(b->_zAxis),
        //     };
        //     int i = 0;

        //     for(auto const& axis: axes) {
        //         if (fabs(gLength.dot(axis)) >= (fabs(computeHalfSizeOnAxis(a, axis) + computeHalfSizeOnAxis(b, axis)))) {
        //             return { false, 1 };
        //         }
        //         i++;
        //     }
        //     return { true, -1 };
        // };

        Intersection intersect(collider_t *a, collider_t *b)  {
            Intersection returnValue = {false, 1};

            if (a->_colliderType == NOCOLLIDER || b->_colliderType == NOCOLLIDER)
                return returnValue;

            if (a->_colliderType == SPHERE && b->_colliderType == SPHERE)
                return intersect(reinterpret_cast<sphereCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));

            if (a->_colliderType == AABB && b->_colliderType == AABB)
                return intersect(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<AABBCollider_t*>(b->_colliderData));

            if (a->_colliderType == AABB && b->_colliderType == SPHERE)
                return intersect(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));

            if (a->_colliderType == SPHERE && b->_colliderType == AABB)
                return intersect(reinterpret_cast<AABBCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));

            // if (a->_colliderType == SPHERE && b->_colliderType == AABB)
            //     return intersect(reinterpret_cast<AABBCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));

            // if (a->_colliderType == CAPSULE && b->_colliderType == CAPSULE)
            //     return intersect(reinterpret_cast<capsuleCollider_t*>(a->_colliderData), reinterpret_cast<capsuleCollider_t*>(b->_colliderData));

            // if (a->_colliderType == OBB && b->_colliderType == OBB)
            //     return intersect(reinterpret_cast<OBBCollider_t*>(a->_colliderData), reinterpret_cast<OBBCollider_t*>(b->_colliderData));

            return returnValue;
        }

        void display(const sphereCollider_t *obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: SphereCollider, position:" + obj->_position.getDisplayInfo() + ", radius: " + std::to_string(obj->_radius));
        }

        void display(const AABBCollider_t *obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: AABBCollider, position:" + obj->_position.getDisplayInfo() + ", size: " + obj->_size.getDisplayInfo());
        }

        void display(const transform_t *obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: transf, pos: " + obj->_position.getDisplayInfo() + ", v: " + obj->_velocity.getDisplayInfo() + ", acc: " + obj->_acceleration.getDisplayInfo());
        }

        void display() {
            for (unsigned long int i = 0; i < _gameObjects.size(); i++) {
                transform_t *transform = _gameObjects[i]->getComponent<transform_t*>(Component::TRANSFORM);
                display(transform);
            }

        }

        void integrate(transform_t *transform, float delta) {
            integrateAcceleration(transform, delta);
            integrateVelocity(transform, delta);
        }

        void integrateVelocity(transform_t *transform, float delta) {
            transform->_position = (transform->_position + (transform->_velocity * delta));
        }

        void integrateAcceleration(transform_t *transform, float delta) {
            transform->_velocity = (transform->_velocity + (transform->_acceleration * delta));
        }

        void init() override {
            ArcLogger::trace("PhysicsModule::init");
        }

        void term() override {
            ArcLogger::trace("PhysicsModule::term");
        }

        void update() override {
            float delta = 1;
            ArcLogger::trace("PhysicsModule::update");
            for (unsigned long int i = 0; i < _gameObjects.size(); i++) {
                transform_t* transform = _gameObjects[i]->getComponent<transform_t *>(Component::TRANSFORM);
                collider_t* collider = _gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);
                integrate(transform, delta);
                _gameObjects[i]->setComponent(Component::COLLIDER, new collider_t(Collider::SPHERE, new sphereCollider_t(EngineMath::Vector3(transform->_position), 1.1)));
                display(transform);
            }
            for (unsigned long int i = 0; i < _gameObjects.size(); i++) {
                collider_t *colliderA = _gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);

                for (unsigned long int j = i + 1; j < _gameObjects.size(); j++) {
                    collider_t *colliderB = _gameObjects[j]->getComponent<collider_t *>(Component::COLLIDER);

                    Intersection inter = intersect(colliderA, colliderB);
                    if (inter.collided) {
                        transform_t *tr = _gameObjects[i]->getComponent<transform_t *>(Component::TRANSFORM);
                        transform_t *trj = _gameObjects[j]->getComponent<transform_t *>(Component::TRANSFORM);

                        std::cout << "COLLIDED" <<   inter.distance << std::endl;
                        _gameObjects[i]->setComponent(Component::TRANSFORM, new transform_t(EngineMath::Vector3(
                            tr->_position.x + inter.distance * (tr->_velocity.x ? (tr->_velocity.x < 0 ? -1 : 1) : 0),
                            tr->_position.y + inter.distance * (tr->_velocity.y ? (tr->_velocity.y < 0 ? -1 : 1) : 0),
                            tr->_position.z + inter.distance * (tr->_velocity.z ? (tr->_velocity.z < 0 ? -1 : 1) : 0)),
                            tr->_velocity * -1,
                            tr->_acceleration * -1));
                        _gameObjects[j]->setComponent(Component::TRANSFORM, new transform_t(EngineMath::Vector3(
                            trj->_position.x + inter.distance * (trj->_velocity.x ? (trj->_velocity.x < 0 ? -1 : 1) : 0),
                            trj->_position.y + inter.distance * (trj->_velocity.y ? (trj->_velocity.y < 0 ? -1 : 1) : 0),
                            trj->_position.z + inter.distance * (trj->_velocity.z ? (trj->_velocity.z < 0 ? -1 : 1) : 0)),
                            trj->_velocity * -1,
                            trj->_acceleration * -1));
                    }
                }
            }
        }

        void testModule() {
            transform_t *tr = new transform_t(EngineMath::Vector3(), EngineMath::Vector3(5,5,5), EngineMath::Vector3(-0.5, -0.5, -0.5));

            sphereCollider_t *sp1 = new sphereCollider_t(EngineMath::Vector3(0.0f,0.0f,0.0f), 1.0f);
            sphereCollider_t *sp2 = new sphereCollider_t(EngineMath::Vector3(2.0f,0.0f,0.0f), 1.0f);
            sphereCollider_t *sp3 = new sphereCollider_t(EngineMath::Vector3(-2.0f,-0.0f,-0.0f), 1.5f);
            sphereCollider_t *sp4 = new sphereCollider_t(EngineMath::Vector3(-1.0f,5.0f,5.0f), 1.1f);

            // collider_t * coll = new collider 

            AABBCollider_t *aa1 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.0f,0.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));
            AABBCollider_t *aa2 = new AABBCollider_t (EngineMath::Vector3(1.0f, 1.0f, 1.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));
            AABBCollider_t *aa3 = new AABBCollider_t (EngineMath::Vector3(0.5f, 0.5f, 0.5f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));
            AABBCollider_t *aa4 = new AABBCollider_t (EngineMath::Vector3(2.0f, 2.0f, 2.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));


            Intersection inter1 = intersect(sp1, sp2);
            Intersection inter2 = intersect(sp1, sp3);

            Intersection inter3 = intersect(aa1, aa2);
            Intersection inter4 = intersect(aa1, aa3);
            Intersection inter5 = intersect(aa1, aa4);


            ASSERT(inter1.collided == false);
            ASSERT(inter1.distance == 0.00f);

            ASSERT(inter2.collided == true);
            ASSERT(inter2.distance == -0.5f);

            ASSERT(inter3.collided == true);
            ASSERT(inter4.collided == true);
            ASSERT(inter5.collided == false);


            integrate(tr, 1);
            ASSERT(tr->_position.x == 4.5f);
            ASSERT(tr->_position.y == 4.5f);
            ASSERT(tr->_position.z == 4.5f);

            integrate(tr, 1);
            ASSERT(tr->_position.x == 8.5f);
            ASSERT(tr->_position.y == 8.5f);
            ASSERT(tr->_position.z == 8.5f);

            integrate(tr, 1);
            ASSERT(tr->_position.x == 12.0f);
            ASSERT(tr->_position.y == 12.0f);
            ASSERT(tr->_position.z == 12.0f);

            integrate(tr, 1);
            ASSERT(tr->_position.x == 15.0f);
            ASSERT(tr->_position.y == 15.0f);
            ASSERT(tr->_position.z == 15.0f);

            integrate(tr, 2);
            ASSERT(tr->_position.x == 19.0f);
            ASSERT(tr->_position.y == 19.0f);
            ASSERT(tr->_position.z == 19.0f);
        }

    private:
};
