//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include "Module.hpp"
#include "ArcLogger.hpp"
#include "collider.hh"
#include "Intersection.hpp"

class PhysicsModule : public Module {
    public:
        PhysicsModule() = default;
        ~PhysicsModule() override = default;
        Intersection intersect(const AABBCollider_t &a, const AABBCollider_t &b) const {
            EngineMath::Vector3 dist1 = b._minExtend - a._maxExtend;
            EngineMath::Vector3 dist2 = a._minExtend - b._maxExtend;
            EngineMath::Vector3 max = dist1.max(dist2);
            float maxVal = max.maxValue();

            return {0 > maxVal, maxVal};
        };
        Intersection intersect(const sphereCollider_t &a, const sphereCollider_t &b) const {
            float radiuses = a._radius + b._radius;
            float distance = a._center.distance(b._center);
            return {radiuses > distance, distance};
        };
        Intersection intersect(const sphereCollider_t &a, const AABBCollider_t &b) const {
            return {false, 1};
        }
        void init() override {
            ArcLogger::trace("PhysicsModule::Init");

        }

        void term() override {
            ArcLogger::trace("PhysicsModule::Terminate");

        }

        void update() override {

        }

private:
};
