#ifndef AABBCOLLIDER_
#define AABBCOLLIDER_

#include "ICollider.hpp"

// class AABBCollider: ICollider {
// public:
//     AABBCollider(EngineMath::Vector3 minExtend, EngineMath::Vector3 maxExtend): _minExtend(minExtend), _maxExtend(maxExtend) {};
//     ~AABBCollider() {};
//     Intersection intersect(const AABBCollider &other) const override {
//         EngineMath::Vector3 dist1 = other.getMinExtend() - _maxExtend;
//         EngineMath::Vector3 dist2 = _minExtend - other.getMaxExtend();
//         EngineMath::Vector3 max = dist1.max(dist2);
//         float maxVal = max.maxValue();

//         return {0 > maxVal, maxVal};
//     };

//     Intersection intersect(const SphereCollider &other) const override;

//     EngineMath::Vector3 getMinExtend() const {return _minExtend;};
//     EngineMath::Vector3 getMaxExtend() const {return _maxExtend;};
// private:
//     EngineMath::Vector3 _minExtend;
//     EngineMath::Vector3 _maxExtend;
// };

#endif