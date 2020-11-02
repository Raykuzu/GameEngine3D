//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include <assert.h>
#include <tuple>
#include <algorithm>
#include "AModule.hpp"
#include "ArcLogger.hpp"
#include "collider.hh"
#include "Intersection.hpp"

#define Square(x) ((x)*(x))
#define ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }

class PhysicsModule : public AModule {
    public:
        explicit PhysicsModule(std::vector<GameObject *> &gameObjects) : AModule(gameObjects) {};
        ~PhysicsModule() override = default;
        Intersection intersect(const AABBCollider_t *a, const AABBCollider_t *b) const {
            EngineMath::Vector3 dist1 = b->_minExtend - a->_maxExtend;
            EngineMath::Vector3 dist2 = a->_minExtend - b->_maxExtend;
            EngineMath::Vector3 max = dist1.max(dist2);
            float maxVal = max.maxValue();

            return {0 > maxVal, maxVal};
        };
        Intersection intersect(const sphereCollider_t *a, const sphereCollider_t *b) const {
            float radiuses = a->_radius + b->_radius;
            float distance = a->_center.distance(b->_center);
            return {radiuses > distance, distance - radiuses};
        };

        Intersection intersect(const AABBCollider_t *a, const sphereCollider_t *b) const {
            float squaredRadius = Square(b->_radius);

            if (b->_center.x < a->_minExtend.x)
                squaredRadius -= Square(b->_center.x - a->_minExtend.x);
            else if (b->_center.x > a->_maxExtend.x)
                squaredRadius -= Square(b->_center.x - a->_maxExtend.x);

            if (b->_center.y < a->_minExtend.y)
                squaredRadius -= Square(b->_center.y - a->_minExtend.y);
            else if (b->_center.y > a->_maxExtend.y)
                squaredRadius -= Square(b->_center.y - a->_maxExtend.y);

            if (b->_center.z < a->_minExtend.z)
                squaredRadius -= Square(b->_center.z - a->_minExtend.z);
            else if (b->_center.z > a->_maxExtend.z)
                squaredRadius -= Square(b->_center.z - a->_maxExtend.z);

            return { squaredRadius > 0, -squaredRadius };
        }

        EngineMath::Vector3 findClosestIntersectionPoint(EngineMath::Vector3 A, EngineMath::Vector3 B, EngineMath::Vector3 Point) const {
            EngineMath::Vector3 AB = B - A;
            float t = (Point - A).dot(AB) / AB.dot(AB);
            return A + AB * std::min(std::max(t, 0.0f), 1.0f);
        }

        std::tuple<EngineMath::Vector3> getCapsuleCenters(const capsuleCollider_t * edge) const {
            EngineMath::Vector3 normal(edge->_tip - edge->_base);
            aNormalized.normalize();
            EngineMath::Vector3 aOffset = aNormalized * a->_radius;
        }

        Intersection intersect(const capsuleCollider_t *a, const capsuleCollider_t *b) const {
            EngineMath::Vector3 aNormalized(a->_tip - a->_base);
            aNormalized.normalize();
            EngineMath::Vector3 aOffset = aNormalized * a->_radius;
            EngineMath::Vector3 aACenter = a->_base + aOffset;
            EngineMath::Vector3 aBCenter = a->_tip - aOffset;

            EngineMath::Vector3 bNormalized(b->_tip - b->_base);
            bNormalized.normalize();
            EngineMath::Vector3 bOffset = bNormalized * b->_radius;
            EngineMath::Vector3 bACenter = b->_base + bOffset;
            EngineMath::Vector3 bBCenter = b->_tip - bOffset;

            EngineMath::Vector3 v0 = bACenter - aACenter;
            EngineMath::Vector3 v1 = bBCenter - aACenter;
            EngineMath::Vector3 v2 = bACenter - aBCenter;
            EngineMath::Vector3 v3 = bBCenter - aBCenter;

            float d0 = v0.dot(v0);
            float d1 = v1.dot(v1);
            float d2 = v2.dot(v2);
            float d3 = v3.dot(v3);

            EngineMath::Vector3 bestA;
            if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1)
                bestA = aBCenter;
            else
                bestA = aACenter;

            EngineMath::Vector3 bestB = findClosestIntersectionPoint(bACenter, bBCenter, bestA);

            bestA = findClosestIntersectionPoint(aACenter, aBCenter, bestB);
            return intersect(new sphereCollider_t(bestA, a->_radius), new sphereCollider_t(bestB, b->_radius));
        };

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

            if (a->_colliderType == CAPSULE && b->_colliderType == CAPSULE)
                return intersect(reinterpret_cast<capsuleCollider_t*>(a->_colliderData), reinterpret_cast<capsuleCollider_t*>(b->_colliderData));

            return returnValue;
        }

        void display(const sphereCollider_t *obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: SphereCollider, center:" + obj->_center.getDisplayInfo() + ", radius: " + std::to_string(obj->_radius));
        }

        void display(const AABBCollider_t *obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: AABBCollider, minExtend:" + obj->_minExtend.getDisplayInfo() + ", maxExtend: " + obj->_maxExtend.getDisplayInfo());
        }

        void display(const transform_t *obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: transf, pos: " + obj->_position.getDisplayInfo() + ", v: " + obj->_velocity.getDisplayInfo() + ", acc: " + obj->_acceleration.getDisplayInfo());
        }

        void display() {
            for (unsigned long int i = 0; i < _gameObjects.size(); i++) {
                transform_t* transform = static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM));
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
                transform_t* transform = static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM));
                collider_t* collider = static_cast<collider_t*>(_gameObjects[i]->getComponent(Component::COLLIDER));

                integrate(transform, delta);
                static_cast<sphereCollider_t*>(collider->_colliderData)->_center = transform->_position;
                display(transform);
            }
            for (unsigned long int i = 0; i < _gameObjects.size(); i++) {
                collider_t* colliderA = static_cast<collider_t*>(_gameObjects[i]->getComponent(Component::COLLIDER));

                for (unsigned long int j = i + 1; j < _gameObjects.size(); j++) {
                    collider_t* colliderB = static_cast<collider_t*>(_gameObjects[j]->getComponent(Component::COLLIDER));

                    Intersection inter = intersect(colliderA, colliderB);
                    if (inter.collided) {
                        transform_t* tr = static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM));
                        transform_t* trj = static_cast<transform_t*>(_gameObjects[j]->getComponent(Component::TRANSFORM));

                        std::cout << "COLLIDED" <<   inter.distance << std::endl;
                        static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM))->_position.x += inter.distance * (tr->_velocity.x ? (tr->_velocity.x < 0 ? -1 : 1) : 0);
                        static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM))->_position.y += inter.distance * (tr->_velocity.y ? (tr->_velocity.y < 0 ? -1 : 1) : 0);
                        static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM))->_position.z += inter.distance * (tr->_velocity.z ? (tr->_velocity.z < 0 ? -1 : 1) : 0);
                        static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM))->_velocity *= -1; 
                        static_cast<transform_t*>(_gameObjects[i]->getComponent(Component::TRANSFORM))->_acceleration *= -1;

                        static_cast<transform_t*>(_gameObjects[j]->getComponent(Component::TRANSFORM))->_position.x += inter.distance * (trj->_velocity.x ? (trj->_velocity.x < 0 ? -1 : 1) : 0);
                        static_cast<transform_t*>(_gameObjects[j]->getComponent(Component::TRANSFORM))->_position.y += inter.distance * (trj->_velocity.y ? (trj->_velocity.y < 0 ? -1 : 1) : 0);
                        static_cast<transform_t*>(_gameObjects[j]->getComponent(Component::TRANSFORM))->_position.z += inter.distance * (trj->_velocity.z ? (trj->_velocity.z < 0 ? -1 : 1) : 0);                        static_cast<transform_t*>(_gameObjects[j]->getComponent(Component::TRANSFORM))->_velocity *= -1; 
                        static_cast<transform_t*>(_gameObjects[j]->getComponent(Component::TRANSFORM))->_velocity *= -1; 
                        static_cast<transform_t*>(_gameObjects[j]->getComponent(Component::TRANSFORM))->_acceleration *= -1;
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

            AABBCollider_t *aa1 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.0f,0.0f), EngineMath::Vector3(1.0f,1.0f,1.0f));
            AABBCollider_t *aa2 = new AABBCollider_t (EngineMath::Vector3(1.0f,1.0f,1.0f), EngineMath::Vector3(2.0f,2.0f,2.0f));
            AABBCollider_t *aa3 = new AABBCollider_t (EngineMath::Vector3(1.0f,0.0f,0.0f), EngineMath::Vector3(2.0f,1.0f,1.0f));
            AABBCollider_t *aa4 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.0f,-2.0f), EngineMath::Vector3(1.0f,1.0f,-1.0f));
            AABBCollider_t *aa5 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.5f,0.0f), EngineMath::Vector3(1.0f,1.5f,1.0f));
            AABBCollider_t *aa6 = new AABBCollider_t (EngineMath::Vector3(0.3f,0.5f,0.7f), EngineMath::Vector3(1.3f,1.5f,1.7f));
            AABBCollider_t *aa7 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.0f,0.0f), EngineMath::Vector3(10.0f,10.0f,10.0f));

            capsuleCollider_t *ca1 = new capsuleCollider_t(EngineMath::Vector3(0.0f, 0.0f, 0.0f), EngineMath::Vector3(0.0f,5.0f,0.0f), 1.0f);
            capsuleCollider_t *ca2 = new capsuleCollider_t(EngineMath::Vector3(3.0f, 3.0f, 3.0f), EngineMath::Vector3(4.0f,4.0f,4.0f), 1.0f);
            capsuleCollider_t *ca3 = new capsuleCollider_t(EngineMath::Vector3(0.9f, 2.0f, 0.0f), EngineMath::Vector3(4.0f,2.0f,0.0f), 1.0f);

            capsuleCollider_t *ca4 = new capsuleCollider_t(EngineMath::Vector3(0.0f, 0.0f, 0.0f), EngineMath::Vector3(0.0f,1.0f,0.0f), 0.1f);
            capsuleCollider_t *ca5 = new capsuleCollider_t(EngineMath::Vector3(2.0f, 0.0f, 0.0f), EngineMath::Vector3(-1.0f,2.0f,0.0f), 0.1f);


            Intersection inter1 = intersect(sp1, sp2);
            Intersection inter2 = intersect(sp1, sp3);

            Intersection inter3 = intersect(aa1, aa2);
            Intersection inter4 = intersect(aa1, aa3);
            Intersection inter5 = intersect(aa1, aa4);
            Intersection inter6 = intersect(aa1, aa5);
            Intersection inter7 = intersect(aa1, aa6);

            Intersection inter8 = intersect(aa1, sp1);
            Intersection inter9 = intersect(aa1, sp1);
            Intersection inter10 = intersect(aa2, sp1);
            Intersection inter11 = intersect(aa7, sp4);

            // Intersection inter12 = intersect(ca1, ca2);
            // Intersection inter13 = intersect(ca1, ca3);

            Intersection inter13 = intersect(ca4, ca5);

            std::cout << "ALED-9: " << inter9.distance << std::endl;
            std::cout << "ALED-10: " << inter10.distance << std::endl;
            std::cout << "ALED-11: " << inter11.distance << std::endl;


            ASSERT(inter1.collided == false);
            ASSERT(inter1.distance == 0.00f);

            ASSERT(inter2.collided == true);
            ASSERT(inter2.distance == -0.5f);

            ASSERT(inter3.collided == false);
            ASSERT(inter3.distance == 0.00f);

            ASSERT(inter4.collided == false);
            ASSERT(inter4.distance == 0.00f);

            ASSERT(inter5.collided == false);
            ASSERT(inter5.distance == 1.00f);

            ASSERT(inter6.collided == true);
            ASSERT(inter6.distance == -0.5f);

            ASSERT(inter7.collided == true);
            ASSERT(inter7.distance == -0.3f);

            ASSERT(inter8.collided == true);

            ASSERT(inter9.collided == true);

            ASSERT(inter10.collided == false);

            ASSERT(inter11.collided == true);

            // ASSERT(inter12.collided == false);

            // ASSERT(inter13.collided == true);


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
