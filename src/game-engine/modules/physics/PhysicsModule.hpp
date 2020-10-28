//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include "AModule.hpp"
#include "ArcLogger.hpp"
#include "collider.hh"
#include "Intersection.hpp"

class PhysicsModule : public AModule {
    public:
        explicit PhysicsModule(std::vector<sharedGO> &gameObjects) : AModule(gameObjects) {};
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
            return {radiuses > distance, distance};
        };
        Intersection intersect(const sphereCollider_t *a, const AABBCollider_t *b) const {
            return {false, 1};
        }

        Intersection intersect(collider_t *a, collider_t *b)  {
            if (a->_colliderType == NOCOLLIDER || b->_colliderType == NOCOLLIDER)
                return {false, 0};
            if (a->_colliderType == SPHERE && b->_colliderType == SPHERE) {
                return intersect(reinterpret_cast<sphereCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));
            }
            if (a->_colliderType == AABB && b->_colliderType == AABB) {
                return intersect(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<AABBCollider_t*>(b->_colliderData));
            }
            return {false, 1};
        }

        void diplay(const sphereCollider_t &obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: SphereCollider, center:" + obj._center.getDisplayInfo() + ", radius: " + std::to_string(obj._radius));
        }

        void diplay(const AABBCollider_t &obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: AABBCollider, minExtend:" + obj._minExtend.getDisplayInfo() + ", maxExtend: " + obj._maxExtend.getDisplayInfo());
        }

        void diplay(const transform_t &obj) {
            ArcLogger::debug("PHYSICS MODULE: OBJ: transform, position: " + obj._position.getDisplayInfo() + ", velocity: " + obj._velocity.getDisplayInfo() + ", acceleration: " + obj._acceleration.getDisplayInfo());
        }

        void integrate(transform_t transform, float delta) {
            // std::cout << 
            ArcLogger::debug("PHYSICS MODULE: OBJ: TMP transform, position:" + (transform._position + transform._velocity * delta).getDisplayInfo());
            transform._position = (transform._position + (transform._velocity * delta));
        }

        void init() override {
            ArcLogger::trace("PhysicsModule::init");
        }

        void term() override {
            ArcLogger::trace("PhysicsModule::term");
        }

        void update() override {
            //ArcLogger::trace("PhysicsModule::update");
        }

    private:
};
