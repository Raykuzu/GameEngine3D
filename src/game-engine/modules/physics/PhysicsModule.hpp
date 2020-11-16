//
// Created by raykuzu on 05/10/2020.
//

#pragma once

#include <assert.h>
#include <utility>
#include <algorithm>
#include <math.h>
#include "AModule.hpp"
#include "ArcLogger.hpp"
#include "collider.hh"
#include "Intersection.hpp"

#define Square(x) ((x)*(x))
#define ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }
typedef std::pair<EngineMath::Vector3, EngineMath::Vector3> CapsuleCenters;
typedef std::pair<EngineMath::Vector3, EngineMath::Vector3> Line;
typedef std::vector<Line> Lines;

class PhysicsModule : public AModule {
    public:
        PhysicsModule() : _linearProjectionPercent(0.45f), _penetrationSlack(0.01f), _impulseIteration(5) {
            _colliders1.reserve(100);
            _colliders2.reserve(100);
            _results.reserve(100);
        };
        ~PhysicsModule() override = default;

        EngineMath::Vector3 getAxis(OBBCollider_t *collider, int index) const {
            float* orientation = collider->_orientation[index];

            return {
                orientation[0],
                orientation[1],
                orientation[2]
            };
        }

        EngineMath::Vector3 getAxis(AABBCollider_t *collider, int index) const {
            (void)collider;

            EngineMath::Vector3 aabbAxes[3] = {
                { 1, 0, 0 },
                { 0, 1, 0 },
                { 0, 0, 1 }
            };

            return aabbAxes[index];
        }

        std::vector<EngineMath::Vector3> getAxes(OBBCollider_t *collider) const {
            return {
                getAxis(collider, 0),
                getAxis(collider, 1),
                getAxis(collider, 2)
            };
        }

        std::vector<EngineMath::Vector3> getAxes(AABBCollider_t *collider) const {
            return {
                getAxis(collider, 0),
                getAxis(collider, 1),
                getAxis(collider, 2)
            };
        }

        EngineMath::Vector3 getClosestPoint(EngineMath::Vector3 a, EngineMath::Vector3 b, EngineMath::Vector3 p) const {
            EngineMath::Vector3 ab = b - a;
            EngineMath::Vector3 pa = p - a;
            float t = pa.dot(ab) / ab.dotSquare();
            return a + ab * std::fmin(std::fmax(t, 0), 1);
        }

        EngineMath::Vector3 getClosestPoint(OBBCollider_t *collider, EngineMath::Vector3 const point) const {
            EngineMath::Vector3 result(point);
            EngineMath::Vector3 direction = point - collider->_position;
            float sizes[3] = { collider->_size.x, collider->_size.y, collider->_size.z };

            for (int i = 0; i != 3; i += 1) {
                EngineMath::Vector3 axis = getAxis(collider, i);
                float distance = direction.dot(axis);

                distance = fmaxf(fminf(distance, sizes[i]), -sizes[i]);
                result += (axis * distance);
            }

            return result;
        }

        EngineMath::Vector3 getClosestPoint(const AABBCollider_t *collider, EngineMath::Vector3 const point) const {
            EngineMath::Vector3 min = getMin(collider);
            EngineMath::Vector3 max = getMax(collider);

            return {
                fminf(fmaxf(point.x, min.x), max.x),
                fminf(fmaxf(point.y, min.y), max.y),
                fminf(fmaxf(point.z, min.z), max.z)
            };
        }

        EngineMath::Vector3 getMin(const AABBCollider_t *collider) const {
            EngineMath::Vector3 p1 = collider->_position + collider->_size;
            EngineMath::Vector3 p2 = collider->_position - collider->_size;

            return EngineMath::Vector3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
        }

        EngineMath::Vector3 getMax(const AABBCollider_t *collider) const {
            EngineMath::Vector3 p1 = collider->_position + collider->_size;
            EngineMath::Vector3 p2 = collider->_position - collider->_size;

            return EngineMath::Vector3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
        }

        std::vector<EngineMath::Vector3> getVertices(AABBCollider_t *collider) const {
            EngineMath::Vector3 min = getMin(collider);
            EngineMath::Vector3 max = getMax(collider);

            return {
                {min.x, max.y, max.z},
                {min.x, max.y, min.z},
                {min.x, min.y, max.z},
                {min.x, min.y, min.z},
                {max.x, max.y, max.z},
                {max.x, max.y, min.z},
                {max.x, min.y, max.z},
                {max.x, min.y, min.z}
            };
        }

        std::vector<EngineMath::Vector3> getVertices(OBBCollider_t *collider) const {
            EngineMath::Vector3 axes[3] = {
                getAxis(collider, 0),
                getAxis(collider, 1),
                getAxis(collider, 2)
            };

            return {
                (collider->_position + axes[0] * collider->_size.x + axes[1] * collider->_size.y + axes[2] * collider->_size.z),
                (collider->_position - axes[0] * collider->_size.x + axes[1] * collider->_size.y + axes[2] * collider->_size.z),
                (collider->_position + axes[0] * collider->_size.x - axes[1] * collider->_size.y + axes[2] * collider->_size.z),
                (collider->_position + axes[0] * collider->_size.x + axes[1] * collider->_size.y - axes[2] * collider->_size.z),
                (collider->_position - axes[0] * collider->_size.x - axes[1] * collider->_size.y - axes[2] * collider->_size.z),
                (collider->_position + axes[0] * collider->_size.x - axes[1] * collider->_size.y - axes[2] * collider->_size.z),
                (collider->_position - axes[0] * collider->_size.x + axes[1] * collider->_size.y - axes[2] * collider->_size.z),
                (collider->_position - axes[0] * collider->_size.x - axes[1] * collider->_size.y + axes[2] * collider->_size.z)
            };
        }

        Lines GetEdges(OBBCollider_t* obb) {
            Lines edges;
            edges.reserve(12);
            std::vector<EngineMath::Vector3> vertices = getVertices(obb);

            int index[12][2] = {{6, 1}, {6, 3}, {6, 4}, {2, 7}, {2, 5}, {2, 0}, {0, 1}, {0, 3}, {7, 1}, {7, 4}, {4, 5}, {5, 3}};

            for (int j = 0; j != 12; j += 1)
                edges.push_back(Line(vertices[index[j][0]], vertices[index[j][1]]));
            return edges;
        }

        Lines GetEdges(AABBCollider_t* aabb) {
            Lines edges;
            edges.reserve(12);
            std::vector<EngineMath::Vector3> vertices = getVertices(aabb);

            int index[12][2] = {{6, 1}, {6, 3}, {6, 4}, {2, 7}, {2, 5}, {2, 0}, {0, 1}, {0, 3}, {7, 1}, {7, 4}, {4, 5}, {5, 3}};

            for (int j = 0; j != 12; j += 1)
                edges.push_back(Line(vertices[index[j][0]], vertices[index[j][1]]));
            return edges;
        }

        std::vector<EngineMath::Plane> GetPlanes(OBBCollider_t *obb) {
            std::vector<EngineMath::Vector3> axes = getAxes(obb);
    
            std::vector<EngineMath::Plane> result;
            result[0] = EngineMath::Plane(axes[0], axes[0].dot(obb->_position + axes[0] * obb->_size.x));
            result[1] = EngineMath::Plane(axes[0] * -1.0f, -(axes[0].dot(obb->_position - axes[0] * obb->_size.x)));
            result[2] = EngineMath::Plane(axes[1], axes[1].dot(obb->_position + axes[1] * obb->_size.y));
            result[3] = EngineMath::Plane(axes[1] * -1.0f, -(axes[1].dot(obb->_position - axes[1] * obb->_size.y)));
            result[4] = EngineMath::Plane(axes[2], axes[2].dot(obb->_position + axes[2] * obb->_size.z));
            result[5] = EngineMath::Plane(axes[2] * -1.0f, -(axes[2].dot(obb->_position - axes[2] * obb->_size.z)));
            result.resize(6);

            return result;
        }

        std::vector<EngineMath::Plane> GetPlanes(AABBCollider_t *aabb) {
            std::vector<EngineMath::Vector3> axes = getAxes(aabb);
    
            std::vector<EngineMath::Plane> result;
            result[0] = EngineMath::Plane(axes[0], axes[0].dot(aabb->_position + axes[0] * aabb->_size.x));
            result[1] = EngineMath::Plane(axes[0] * -1.0f, -(axes[0].dot(aabb->_position - axes[0] * aabb->_size.x)));
            result[2] = EngineMath::Plane(axes[1], axes[1].dot(aabb->_position + axes[1] * aabb->_size.y));
            result[3] = EngineMath::Plane(axes[1] * -1.0f, -(axes[1].dot(aabb->_position - axes[1] * aabb->_size.y)));
            result[4] = EngineMath::Plane(axes[2], axes[2].dot(aabb->_position + axes[2] * aabb->_size.z));
            result[5] = EngineMath::Plane(axes[2] * -1.0f, -(axes[2].dot(aabb->_position - axes[2] * aabb->_size.z)));
            result.resize(6);

            return result;
        }

        bool ClipToPlane(const EngineMath::Plane& plane, const Line& line, EngineMath::Vector3* outPoint) {
            EngineMath::Vector3 ab = line.first - line.second;
            float nAB = (plane.normal).dot(ab);

            if (CMP(nAB, 0))
                return false;

            float nA = plane.normal.dot(line.first);
            float t = (plane.distance - nA) / nAB;
                if (t >= 0.0f && t <= 1.0f) {
                if (outPoint != 0)
                    *outPoint = line.first + ab * t;
                return true;
            }
            return false;
        }

        EngineMath::interval_t getInterval(OBBCollider_t *collider, const EngineMath::Vector3 axis) const {
            std::vector<EngineMath::Vector3> vertices = getVertices(collider);
            float firstVertexDot = axis.dot(vertices[0]);
            EngineMath::interval_t result = { firstVertexDot, firstVertexDot };

            for(auto const& vertex: vertices) {
                float projection = axis.dot(vertex);
                result.min = fminf(projection, result.min);
                result.max = fmaxf(projection, result.max);
            }

            return result;
        }

        EngineMath::interval_t getInterval(AABBCollider_t *collider, const EngineMath::Vector3 axis) const {
            std::vector<EngineMath::Vector3> vertices = getVertices(collider);
            float firstVertexDot = axis.dot(vertices[0]);
            EngineMath::interval_t result = { firstVertexDot, firstVertexDot };

            for(auto const& vertex: vertices) {
                float projection = axis.dot(vertex);
                result.min = fminf(projection, result.min);
                result.max = fmaxf(projection, result.max);
            }

            return result;
        }

        bool areCollidersIntersectingOnAxis(AABBCollider_t *a, OBBCollider_t *b, const EngineMath::Vector3 axis) const {
            EngineMath::interval_t aInterval = getInterval(a, axis);
            EngineMath::interval_t bInterval = getInterval(b, axis);

            return ((bInterval.min <= aInterval.max) && (aInterval.min <= bInterval.max));
        }

        bool areCollidersIntersectingOnAxis(OBBCollider_t *a, OBBCollider_t *b, const EngineMath::Vector3 axis) const {
            EngineMath::interval_t aInterval = getInterval(a, axis);
            EngineMath::interval_t bInterval = getInterval(b, axis);

            return ((bInterval.min <= aInterval.max) && (aInterval.min <= bInterval.max));
        }

        Intersection intersect(const AABBCollider_t *a, const AABBCollider_t *b) const {
            EngineMath::Vector3 minA = getMin(a);
            EngineMath::Vector3 minB = getMin(b);
            EngineMath::Vector3 maxA = getMax(a);
            EngineMath::Vector3 maxB = getMax(b);

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
            EngineMath::Vector3 closestPoint = getClosestPoint(a, b->_position);
            float distSquare = (b->_position - closestPoint).dotSquare();
            float radiusSquare = b->_radius * b->_radius;

            return { distSquare < radiusSquare, distSquare - radiusSquare };
        }

        Intersection intersect(OBBCollider_t *a, const sphereCollider_t *b) const {
            EngineMath::Vector3 closestPoint = getClosestPoint(a, b->_position);
            float distSquare = (b->_position - closestPoint).dotSquare();
            float radiusSquare = b->_radius * b->_radius;

            return { distSquare < radiusSquare, distSquare - radiusSquare };
        }

        Intersection intersect(OBBCollider_t *a, AABBCollider_t *b) const {
            EngineMath::Vector3 obbAxes[3] = {
                getAxis(a, 0),
                getAxis(a, 1),
                getAxis(a, 2)
            };
            EngineMath::Vector3 aabbAxes[3] = {
                { 1, 0, 0 },
                { 0, 1, 0 },
                { 0, 0, 1 }
            };
            std::vector<EngineMath::Vector3> axesToTest {
                aabbAxes[0],
                aabbAxes[1],
                aabbAxes[2],
                obbAxes[0],
                obbAxes[1],
                obbAxes[2],
                aabbAxes[0].cross(obbAxes[0]),
                aabbAxes[0].cross(obbAxes[1]),
                aabbAxes[0].cross(obbAxes[2]),
                aabbAxes[1].cross(obbAxes[0]),
                aabbAxes[1].cross(obbAxes[1]),
                aabbAxes[1].cross(obbAxes[2]),
                aabbAxes[2].cross(obbAxes[0]),
                aabbAxes[2].cross(obbAxes[1]),
                aabbAxes[2].cross(obbAxes[2])
            };

            for(auto const& axisToTest: axesToTest) {
                if (!areCollidersIntersectingOnAxis(b, a, axisToTest))
                    return { false, 1 };
            }
            return { true, -1 };
        }

        Intersection intersect(OBBCollider_t *a, OBBCollider_t *b) const {
            EngineMath::Vector3 aAxes[3] = {
                getAxis(a, 0),
                getAxis(a, 1),
                getAxis(a, 2)
            };
            EngineMath::Vector3 bAxes[3] = {
                getAxis(b, 0),
                getAxis(b, 1),
                getAxis(b, 2)
            };
            std::vector<EngineMath::Vector3> axesToTest {
                aAxes[0],
                aAxes[1],
                aAxes[2],
                bAxes[0],
                bAxes[1],
                bAxes[2],
                aAxes[0].cross(bAxes[0]),
                aAxes[0].cross(bAxes[1]),
                aAxes[0].cross(bAxes[2]),
                aAxes[1].cross(bAxes[0]),
                aAxes[1].cross(bAxes[1]),
                aAxes[1].cross(bAxes[2]),
                aAxes[2].cross(bAxes[0]),
                aAxes[2].cross(bAxes[1]),
                aAxes[2].cross(bAxes[2])
            };

            for(auto const& axisToTest: axesToTest) {
                if (!areCollidersIntersectingOnAxis(b, a, axisToTest))
                    return { false, 1 };
            }
            return { true, -1 };
        }

        Intersection intersect(rayCollider_t *a, sphereCollider_t *b) const {
            EngineMath::Vector3 rayToSphere = b->_position - a->_origin;
            float radiusSq = b->_radius * b->_radius;
            float rayToSphereSq = rayToSphere.dot(rayToSphere);
            float aVec = rayToSphere.dot(a->_direction);
            float bSq = rayToSphereSq - (aVec * aVec);

            if (radiusSq - bSq < 0.0f)
                return { false, 1 };

            float c = sqrt(radiusSq - bSq);

            if (rayToSphereSq < radiusSq)
                c *= -1;
            return { true, aVec - c };
        }

        Intersection intersect(rayCollider_t *a, AABBCollider_t *b) const {
            EngineMath::Vector3 min = getMin(b);
            EngineMath::Vector3 max = getMax(b);
            float tList[6] = { 0, 0, 0, 0, 0, 0 };

            for (int i = 0; i != 3; i += 1) {
                float direction = a->_direction[i];

                if (CMP(a->_direction[i], 0)) {
                    if (a->_origin.x < min.x || a->_origin.x > max.x) {
                        return { false, -1 };
                    }
                    direction = 0.00001f;
                }
                tList[i * 2 + 0] = (a->_origin[i] + min[i]) / direction;
                tList[i * 2 + 1] = (a->_origin[i] + max[i]) / direction;
            }

            float tLargestMin = fmaxf(fmaxf(fminf(tList[0], tList[1]), fminf(tList[2], tList[3])), fminf(tList[4], tList[5]));
            float tSmallestMax = fminf(fminf(fmaxf(tList[0], tList[1]), fmaxf(tList[2], tList[3])), fmaxf(tList[4], tList[5]));

            if (tSmallestMax < 0.0f || tLargestMin > tSmallestMax)
                return { false, -1 };
            return { true, tLargestMin < 0.0f ? tSmallestMax : tLargestMin };
        }

        Intersection intersect(rayCollider_t *a, OBBCollider_t *b) const {
            EngineMath::Vector3 xAxis = getAxis(b, 0);
            EngineMath::Vector3 yAxis = getAxis(b, 1);
            EngineMath::Vector3 zAxis = getAxis(b, 2);

            EngineMath::Vector3 p = b->_position - a->_origin;

            EngineMath::Vector3 dirAxisAngle = {
                xAxis.dot(a->_direction),
                yAxis.dot(a->_direction),
                zAxis.dot(a->_direction)
            };

            EngineMath::Vector3 e = {
                xAxis.dot(p),
                yAxis.dot(p),
                zAxis.dot(p)
            };

            float tList[6] = { 0, 0, 0, 0, 0, 0 };
            for (int i = 0; i != 3; i += 1) {
                float angle = dirAxisAngle[i];

                if (CMP(dirAxisAngle[i], 0)) {
                    if (-e[i] - b->_size[i] > 0 || -e[i] + b->_size[i] < 0) {
                        return { false, -1 };
                    }
                    angle = 0.00001f;
                }
                tList[i * 2 + 0] = (e[i] + b->_size[i]) / angle;
                tList[i * 2 + 1] = (e[i] - b->_size[i]) / angle;
            }

            float tLargestMin = fmaxf(fmaxf(fminf(tList[0], tList[1]), fminf(tList[2], tList[3])), fminf(tList[4], tList[5]));
            float tSmallestMax = fminf(fminf(fmaxf(tList[0], tList[1]), fmaxf(tList[2], tList[3])), fmaxf(tList[4], tList[5]));

            if (tSmallestMax < 0.0f || tLargestMin > tSmallestMax)
                return { false, -1 };
            return { true, tLargestMin < 0.0f ? tSmallestMax : tLargestMin };
        }

        Intersection intersect(capsuleCollider_t *a, capsuleCollider_t *b) const {
            EngineMath::Vector3 aNormalVec = a->_tip - a->_base;
            aNormalVec.normalize();
            EngineMath::Vector3 aCenterOffset = aNormalVec * a->_radius;
            EngineMath::Vector3 aBotCenter = a->_base + aCenterOffset;
            EngineMath::Vector3 aTopCenter = a->_tip - aCenterOffset;

            EngineMath::Vector3 bNormalVec = b->_tip - b->_base;
            bNormalVec.normalize();
            EngineMath::Vector3 bCenterOffset = bNormalVec * b->_radius;
            EngineMath::Vector3 bBotCenter = b->_base + bCenterOffset;
            EngineMath::Vector3 bTopCenter = b->_tip - bCenterOffset;

            float botBotDist = static_cast<EngineMath::Vector3>(bBotCenter - aBotCenter).dotSquare();
            float topTopDist = static_cast<EngineMath::Vector3>(bTopCenter - aTopCenter).dotSquare();
            float topBotDist = static_cast<EngineMath::Vector3>(bTopCenter - aBotCenter).dotSquare();
            float botTopDist = static_cast<EngineMath::Vector3>(bBotCenter - aTopCenter).dotSquare();

            EngineMath::Vector3 closestA = (botTopDist < botBotDist || botTopDist < topBotDist || topTopDist < botBotDist || topTopDist < topBotDist) ? aTopCenter : aBotCenter;

            EngineMath::Vector3 closestB = getClosestPoint(bBotCenter, bTopCenter, closestA);
            closestA = getClosestPoint(aBotCenter, aTopCenter, closestB);

            sphereCollider_t aSphere(closestA, a->_radius);
            sphereCollider_t bSphere(closestB, b->_radius);
            return intersect(&aSphere, &bSphere);
        }

        Intersection intersect(capsuleCollider_t *a, AABBCollider_t *b) const {
            EngineMath::Vector3 aNormalVec = a->_tip - a->_base;
            aNormalVec.normalize();
            EngineMath::Vector3 aCenterOffset = aNormalVec * a->_radius;
            EngineMath::Vector3 aBotCenter = a->_base + aCenterOffset;
            EngineMath::Vector3 aTopCenter = a->_tip - aCenterOffset;

            std::vector<EngineMath::Vector3> vertices = getVertices(b);

            EngineMath::Vector3 bestPoint;
            float minDistance;
            float currentDistance;
            EngineMath::Vector3 currentPoint;

            for (int i = 0; i != 8; i += 1) {
                currentPoint = getClosestPoint(aBotCenter, aTopCenter, vertices[i]);
                currentDistance = std::fabs(static_cast<EngineMath::Vector3>(vertices[i] - currentPoint).dotSquare());

                if (i == 0 || currentDistance < minDistance) {
                    minDistance = currentDistance;
                    bestPoint = currentPoint;
                }
            }

            sphereCollider_t aSphere(bestPoint, a->_radius);
            return intersect(b, &aSphere);
        }

        Intersection intersect(capsuleCollider_t *a, OBBCollider_t *b) const {
            EngineMath::Vector3 aNormalVec = a->_tip - a->_base;
            aNormalVec.normalize();
            EngineMath::Vector3 aCenterOffset = aNormalVec * a->_radius;
            EngineMath::Vector3 aBotCenter = a->_base + aCenterOffset;
            EngineMath::Vector3 aTopCenter = a->_tip - aCenterOffset;

            std::vector<EngineMath::Vector3> vertices = getVertices(b);

            EngineMath::Vector3 bestPoint;
            float minDistance;
            float currentDistance;
            EngineMath::Vector3 currentPoint;

            for (int i = 0; i != 8; i += 1) {
                currentPoint = getClosestPoint(aBotCenter, aTopCenter, vertices[i]);
                currentDistance = std::fabs(static_cast<EngineMath::Vector3>(vertices[i] - currentPoint).dotSquare());

                if (i == 0 || currentDistance < minDistance) {
                    minDistance = currentDistance;
                    bestPoint = currentPoint;
                }
            }

            sphereCollider_t aSphere(bestPoint, a->_radius);
            return intersect(b, &aSphere);
        }

        Intersection intersect(capsuleCollider_t *a, sphereCollider_t *b) const {
            EngineMath::Vector3 aNormalVec = a->_tip - a->_base;
            aNormalVec.normalize();
            EngineMath::Vector3 aCenterOffset = aNormalVec * a->_radius;
            EngineMath::Vector3 aBotCenter = a->_base + aCenterOffset;
            EngineMath::Vector3 aTopCenter = a->_tip - aCenterOffset;

            EngineMath::Vector3 closestA = getClosestPoint(aBotCenter, aTopCenter, b->_position);

            sphereCollider_t aSphere(closestA, a->_radius);
            return intersect(&aSphere, b);
        }

        Intersection intersect(rayCollider_t *a, capsuleCollider_t *b) const {
            EngineMath::Vector3 tipToBase = b->_base - b->_tip;
            EngineMath::Vector3 size(b->_radius, b->_radius, (tipToBase / 2).z);
            AABBCollider_t aabb(tipToBase / 2, size);

            return intersect(a, &aabb);
        }

        Intersection intersect(collider_t *a, collider_t *b) const {
            Intersection returnValue = {false, 1};

            if (a->_colliderType == NOCOLLIDER || b->_colliderType == NOCOLLIDER)
                return returnValue;

            if (a->_colliderType == SPHERE && b->_colliderType == SPHERE)
                return intersect(reinterpret_cast<sphereCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));

            if (a->_colliderType == SPHERE && b->_colliderType == AABB)
                return intersect(reinterpret_cast<AABBCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));

            if (a->_colliderType == SPHERE && b->_colliderType == OBB)
                return intersect(reinterpret_cast<OBBCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));

            if (a->_colliderType == SPHERE && b->_colliderType == CAPSULE)
                return intersect(reinterpret_cast<capsuleCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));

            if (a->_colliderType == SPHERE && b->_colliderType == RAY)
                return intersect(reinterpret_cast<rayCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));

            if (a->_colliderType == AABB && b->_colliderType == AABB)
                return intersect(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<AABBCollider_t*>(b->_colliderData));

            if (a->_colliderType == AABB && b->_colliderType == SPHERE)
                return intersect(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));

            if (a->_colliderType == AABB && b->_colliderType == OBB)
                return intersect(reinterpret_cast<OBBCollider_t*>(b->_colliderData), reinterpret_cast<AABBCollider_t*>(a->_colliderData));

            if (a->_colliderType == AABB && b->_colliderType == CAPSULE)
                return intersect(reinterpret_cast<capsuleCollider_t*>(b->_colliderData), reinterpret_cast<AABBCollider_t*>(a->_colliderData));

            if (a->_colliderType == AABB && b->_colliderType == RAY)
                return intersect(reinterpret_cast<rayCollider_t*>(b->_colliderData), reinterpret_cast<AABBCollider_t*>(a->_colliderData));

            if (a->_colliderType == OBB && b->_colliderType == OBB)
                return intersect(reinterpret_cast<OBBCollider_t*>(b->_colliderData), reinterpret_cast<OBBCollider_t*>(a->_colliderData));

            if (a->_colliderType == OBB && b->_colliderType == AABB)
                return intersect(reinterpret_cast<OBBCollider_t*>(a->_colliderData), reinterpret_cast<AABBCollider_t*>(b->_colliderData));

            if (a->_colliderType == OBB && b->_colliderType == SPHERE)
                return intersect(reinterpret_cast<OBBCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));

            if (a->_colliderType == OBB && b->_colliderType == CAPSULE)
                return intersect(reinterpret_cast<capsuleCollider_t*>(b->_colliderData), reinterpret_cast<OBBCollider_t*>(a->_colliderData));

            if (a->_colliderType == OBB && b->_colliderType == RAY)
                return intersect(reinterpret_cast<rayCollider_t*>(b->_colliderData), reinterpret_cast<OBBCollider_t*>(a->_colliderData));

            if (a->_colliderType == CAPSULE && b->_colliderType == CAPSULE)
                return intersect(reinterpret_cast<capsuleCollider_t*>(a->_colliderData), reinterpret_cast<capsuleCollider_t*>(b->_colliderData));

            if (a->_colliderType == CAPSULE && b->_colliderType == SPHERE)
                return intersect(reinterpret_cast<capsuleCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));

            if (a->_colliderType == CAPSULE && b->_colliderType == AABB)
                return intersect(reinterpret_cast<capsuleCollider_t*>(a->_colliderData), reinterpret_cast<AABBCollider_t*>(b->_colliderData));

            if (a->_colliderType == CAPSULE && b->_colliderType == OBB)
                return intersect(reinterpret_cast<capsuleCollider_t*>(a->_colliderData), reinterpret_cast<OBBCollider_t*>(b->_colliderData));

            if (a->_colliderType == CAPSULE && b->_colliderType == RAY)
                return intersect(reinterpret_cast<rayCollider_t*>(b->_colliderData), reinterpret_cast<capsuleCollider_t*>(a->_colliderData));

            if (a->_colliderType == RAY && b->_colliderType == SPHERE)
                return intersect(reinterpret_cast<rayCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));

            if (a->_colliderType == RAY && b->_colliderType == AABB)
                return intersect(reinterpret_cast<rayCollider_t*>(a->_colliderData), reinterpret_cast<AABBCollider_t*>(b->_colliderData));

            if (a->_colliderType == RAY && b->_colliderType == OBB)
                return intersect(reinterpret_cast<rayCollider_t*>(a->_colliderData), reinterpret_cast<OBBCollider_t*>(b->_colliderData));

            if (a->_colliderType == RAY && b->_colliderType == CAPSULE)
                return intersect(reinterpret_cast<rayCollider_t*>(a->_colliderData), reinterpret_cast<capsuleCollider_t*>(b->_colliderData));

            return returnValue;
        }
//--------------------------------        
//Manifold Part
//--------------------------------        

        typedef struct CollisionManifold {
            bool colliding;
            EngineMath::Vector3 normal;
            float depth;
            std::vector<EngineMath::Vector3> contacts;
        } CollisionManifold_t;

        void ResetCollisionManifold(CollisionManifold* result) {
            if (result != 0) {
                result->colliding = false;
                result->normal = EngineMath::Vector3(0, 0, 1);
                result->depth = FLT_MAX;
                result->contacts.clear();
            }
        }

        CollisionManifold FindCollisionFeatures(const sphereCollider_t *a, const sphereCollider_t *b) {
            CollisionManifold result;
            ResetCollisionManifold(&result);

            float radius = a->_radius + b->_radius;
            EngineMath::Vector3 distanceVec = b->_position - a->_position;
            float distanceSq = MagnitudeSq(distanceVec);
            float radiusSq = radius * radius;

            if (distanceSq - radiusSq > 0 || distanceSq == 0.0f)
                return result;

            distanceVec.normalize();
            result.colliding = true;
            result.normal = distanceVec;
            result.depth = fabsf(Magnitude(distanceVec) - radius) * 0.5f;

            float dtp = a->_radius - result.depth;
            EngineMath::Vector3 contact = a->_position + distanceVec * dtp;

            result.contacts.push_back(contact);

            return result;
        }

        CollisionManifold FindCollisionFeatures(const AABBCollider_t *a, const sphereCollider_t *b) {
            CollisionManifold result;
            ResetCollisionManifold(&result);

            EngineMath::Vector3 closestPoint = getClosestPoint(a, b->_position);
            float distSq = MagnitudeSq(closestPoint - b->_position);
            float radiusSq = b->_radius * b->_radius;

            if (distSq > radiusSq)
                return result;

            EngineMath::Vector3 normal; 
            if (CMP(distSq, 0.0f)) {
                float mSq = MagnitudeSq(closestPoint - a->_position);
                if (CMP(mSq, 0.0f))
                    return result;
                normal = (closestPoint - a->_position);
                normal.normalize();
            } else {
                normal = (b->_position - closestPoint);
                normal.normalize();
            }

            EngineMath::Vector3 outsidePoint = b->_position - normal * b->_radius;
            float distance = Magnitude(closestPoint - outsidePoint);

            result.colliding = true;
            result.contacts.push_back(closestPoint + (outsidePoint - closestPoint) * 0.5f);
            result.normal = normal;
            result.depth = distance * 0.5f;

            return result;
        }

        CollisionManifold FindCollisionFeatures(OBBCollider_t* a, sphereCollider_t* b) {
            CollisionManifold result;
            ResetCollisionManifold(&result);

            EngineMath::Vector3 closestPoint = getClosestPoint(a, b->_position);
            float distSq = MagnitudeSq(closestPoint - b->_position);
            float radiusSq = b->_radius * b->_radius;

            if (distSq > radiusSq)
                return result;

            EngineMath::Vector3 normal; 
            if (CMP(distSq, 0.0f)) {
                float mSq = MagnitudeSq(closestPoint - a->_position);
                if (CMP(mSq, 0.0f))
                    return result;
                normal = (closestPoint - a->_position);
                normal.normalize();
            } else {
                normal = (b->_position - closestPoint);
                normal.normalize();
            }

            EngineMath::Vector3 outsidePoint = b->_position - normal * b->_radius;
            float distance = Magnitude(closestPoint - outsidePoint);

            result.colliding = true;
            result.contacts.push_back(closestPoint + (outsidePoint - closestPoint) * 0.5f);
            result.normal = normal;
            result.depth = distance * 0.5f;

            return result;
        }

        bool PointInOBB(const EngineMath::Vector3& point, OBBCollider_t* obb) {
            EngineMath::Vector3 dir = point - obb->_position;

            for (int i = 0; i < 3; i += 1) {
                EngineMath::Vector3 axis = getAxis(obb, i);
                float distance = dir.dot(axis);

                if (distance > obb->_size[i]) {
                    return false;
                }
                if (distance < -obb->_size[i]) {
                    return false;
                }
            }

            return true;
        }

        bool PointInAABB(const EngineMath::Vector3& point, AABBCollider_t* aabb) {
            EngineMath::Vector3 dir = point - aabb->_position;

            for (int i = 0; i < 3; i += 1) {
                EngineMath::Vector3 axis = getAxis(aabb, i);
                float distance = dir.dot(axis);

                if (distance > aabb->_size[i]) {
                    return false;
                }
                if (distance < -aabb->_size[i]) {
                    return false;
                }
            }

            return true;
        }

        std::vector<EngineMath::Vector3> ClipEdgesToOBB(const Lines& edges, OBBCollider_t* obb) {
            std::vector<EngineMath::Vector3> result;
            EngineMath::Vector3 intersection;
            std::vector<EngineMath::Plane> planes = GetPlanes(obb);

            result.reserve(edges.size());

            for (unsigned int i = 0; i < planes.size(); i += 1) {
                for (unsigned int j = 0; j < edges.size(); j += 1) {
                    if (ClipToPlane(planes[i], edges[j], &intersection)) {
                        if (PointInOBB(intersection, obb)) {
                            result.push_back(intersection);
                        }
                    }
                }
            }
            return result;
        }

        std::vector<EngineMath::Vector3> ClipEdgesToAABB(const Lines& edges, AABBCollider_t* aabb) {
            std::vector<EngineMath::Vector3> result;
            EngineMath::Vector3 intersection;
            std::vector<EngineMath::Plane> planes = GetPlanes(aabb);

            result.reserve(edges.size());

            for (unsigned int i = 0; i < planes.size(); i += 1) {
                for (unsigned int j = 0; j < edges.size(); j += 1) {
                    if (ClipToPlane(planes[i], edges[j], &intersection)) {
                        if (PointInAABB(intersection, aabb)) {
                            result.push_back(intersection);
                        }
                    }
                }
            }
            return result;
        }

        float PenetrationDepth(OBBCollider_t* o1, OBBCollider_t* o2, const EngineMath::Vector3& axis, bool* outShouldFlip) {
            EngineMath::Vector3 normalizedAxis = axis;
            normalizedAxis.normalize();

            EngineMath::interval_t interval1 = getInterval(o1, normalizedAxis);
            EngineMath::interval_t interval2 = getInterval(o2, normalizedAxis);

            if (!((interval2.min <= interval1.max) && (interval1.min <= interval2.max)))
               return 0.0f;

            float distance1 = interval1.max - interval1.min;
            float distance2 = interval2.max - interval2.min;
            float min = fminf(interval1.min, interval2.min);
            float max = fmaxf(interval1.max, interval2.max);
            float length = max - min;
            if (outShouldFlip != 0)
                *outShouldFlip = (interval2.min < interval1.min);

            return (distance1 + distance2) - length;
        }

        float PenetrationDepth(AABBCollider_t* a1, OBBCollider_t* o2, const EngineMath::Vector3& axis, bool* outShouldFlip) {
            EngineMath::Vector3 normalizedAxis = axis;
            normalizedAxis.normalize();

            EngineMath::interval_t interval1 = getInterval(a1, normalizedAxis);
            EngineMath::interval_t interval2 = getInterval(o2, normalizedAxis);

            if (!((interval2.min <= interval1.max) && (interval1.min <= interval2.max)))
               return 0.0f;

            float distance1 = interval1.max - interval1.min;
            float distance2 = interval2.max - interval2.min;
            float min = fminf(interval1.min, interval2.min);
            float max = fmaxf(interval1.max, interval2.max);
            float length = max - min;
            if (outShouldFlip != 0)
                *outShouldFlip = (interval2.min < interval1.min);

            return (distance1 + distance2) - length;
        }

        float PenetrationDepth(AABBCollider_t* a1, AABBCollider_t* a2, const EngineMath::Vector3& axis, bool* outShouldFlip) {
            EngineMath::Vector3 normalizedAxis = axis;
            normalizedAxis.normalize();

            EngineMath::interval_t interval1 = getInterval(a1, normalizedAxis);
            EngineMath::interval_t interval2 = getInterval(a2, normalizedAxis);

            if (!((interval2.min <= interval1.max) && (interval1.min <= interval2.max)))
               return 0.0f;

            float distance1 = interval1.max - interval1.min;
            float distance2 = interval2.max - interval2.min;
            float min = fminf(interval1.min, interval2.min);
            float max = fmaxf(interval1.max, interval2.max);
            float length = max - min;
            if (outShouldFlip != 0)
                *outShouldFlip = (interval2.min < interval1.min);

            return (distance1 + distance2) - length;
        }

        CollisionManifold FindCollisionFeatures(AABBCollider_t* a, AABBCollider_t* b) {
            CollisionManifold result;
            ResetCollisionManifold(&result);

            std::vector<EngineMath::Vector3> aAxes = getAxes(a);
            std::vector<EngineMath::Vector3> bAxes = getAxes(b);
            EngineMath::Vector3* hitNormal = 0;
            bool shouldFlip;

            std::vector<EngineMath::Vector3> axesToTest {
                aAxes[0],
                aAxes[1],
                aAxes[2],
                bAxes[0],
                bAxes[1],
                bAxes[2],
                aAxes[0].cross(bAxes[0]),
                aAxes[0].cross(bAxes[1]),
                aAxes[0].cross(bAxes[2]),
                aAxes[1].cross(bAxes[0]),
                aAxes[1].cross(bAxes[1]),
                aAxes[1].cross(bAxes[2]),
                aAxes[2].cross(bAxes[0]),
                aAxes[2].cross(bAxes[1]),
                aAxes[2].cross(bAxes[2])
            };

            for (int i = 0; i < 15; i += 1) {
                if (MagnitudeSq(axesToTest[i]) < 0.001f)
                    continue;

                float depth = PenetrationDepth(a, b, axesToTest[i], &shouldFlip);
                if (depth <= 0.0f) {
                    return result;
                } else if (depth <result.depth) {
                    if (shouldFlip)
                        axesToTest[i] = axesToTest[i] * -1.0f;

                    result.depth = depth;
                    hitNormal = &axesToTest[i];
                }
            }

            if (hitNormal == 0)
                return result;

            EngineMath::Vector3 axis = *hitNormal;
            axis.normalize();
            std::vector<EngineMath::Vector3> c1 = ClipEdgesToAABB(GetEdges(b), a);
            std::vector<EngineMath::Vector3> c2 = ClipEdgesToAABB(GetEdges(a), b);

            result.contacts.reserve(c1.size() + c2.size());
            result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
            result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

            EngineMath::interval_t i = getInterval(a, axis);
            float distance = (i.max - i.min)* 0.5f - result.depth * 0.5f;
            EngineMath::Vector3 pointOnPlane = a->_position + axis * distance;

            for (int i = result.contacts.size() - 1; i >= 0; i -= 1) {
                EngineMath::Vector3 contact = result.contacts[i];
                result.contacts[i] = contact + (axis * (axis.dot(pointOnPlane - contact)));

                for (int j = result.contacts.size() - 1; j > i; j -= 1) {
                    if (MagnitudeSq(result.contacts[j] - result.contacts[i]) < 0.0001f) {
                        result.contacts.erase(result.contacts.begin() + j);
                        break;
                    }
                }
            }

            result.colliding = true;
            result.normal = axis;

            return result;
        }

        CollisionManifold FindCollisionFeatures(AABBCollider_t* a, OBBCollider_t* b) {
            CollisionManifold result;
            ResetCollisionManifold(&result);

            std::vector<EngineMath::Vector3> aAxes = getAxes(a);
            std::vector<EngineMath::Vector3> bAxes = getAxes(b);
            EngineMath::Vector3* hitNormal = 0;
            bool shouldFlip;

            std::vector<EngineMath::Vector3> axesToTest {
                aAxes[0],
                aAxes[1],
                aAxes[2],
                bAxes[0],
                bAxes[1],
                bAxes[2],
                aAxes[0].cross(bAxes[0]),
                aAxes[0].cross(bAxes[1]),
                aAxes[0].cross(bAxes[2]),
                aAxes[1].cross(bAxes[0]),
                aAxes[1].cross(bAxes[1]),
                aAxes[1].cross(bAxes[2]),
                aAxes[2].cross(bAxes[0]),
                aAxes[2].cross(bAxes[1]),
                aAxes[2].cross(bAxes[2])
            };

            for (int i = 0; i < 15; i += 1) {
                if (MagnitudeSq(axesToTest[i]) < 0.001f)
                    continue;

                float depth = PenetrationDepth(a, b, axesToTest[i], &shouldFlip);
                if (depth <= 0.0f) {
                    return result;
                } else if (depth <result.depth) {
                    if (shouldFlip)
                        axesToTest[i] = axesToTest[i] * -1.0f;

                    result.depth = depth;
                    hitNormal = &axesToTest[i];
                }
            }

            if (hitNormal == 0)
                return result;

            EngineMath::Vector3 axis = *hitNormal;
            axis.normalize();
            std::vector<EngineMath::Vector3> c1 = ClipEdgesToAABB(GetEdges(b), a);
            std::vector<EngineMath::Vector3> c2 = ClipEdgesToOBB(GetEdges(a), b);

            result.contacts.reserve(c1.size() + c2.size());
            result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
            result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

            EngineMath::interval_t i = getInterval(a, axis);
            float distance = (i.max - i.min)* 0.5f - result.depth * 0.5f;
            EngineMath::Vector3 pointOnPlane = a->_position + axis * distance;

            for (int i = result.contacts.size() - 1; i >= 0; i -= 1) {
                EngineMath::Vector3 contact = result.contacts[i];
                result.contacts[i] = contact + (axis * (axis.dot(pointOnPlane - contact)));

                for (int j = result.contacts.size() - 1; j > i; j -= 1) {
                    if (MagnitudeSq(result.contacts[j] - result.contacts[i]) < 0.0001f) {
                        result.contacts.erase(result.contacts.begin() + j);
                        break;
                    }
                }
            }

            result.colliding = true;
            result.normal = axis;

            return result;
        }

        CollisionManifold FindCollisionFeatures(OBBCollider_t* a, OBBCollider_t* b) {
            CollisionManifold result;
            ResetCollisionManifold(&result);

            std::vector<EngineMath::Vector3> aAxes = getAxes(a);
            std::vector<EngineMath::Vector3> bAxes = getAxes(b);
            EngineMath::Vector3* hitNormal = 0;
            bool shouldFlip;

            std::vector<EngineMath::Vector3> axesToTest {
                aAxes[0],
                aAxes[1],
                aAxes[2],
                bAxes[0],
                bAxes[1],
                bAxes[2],
                aAxes[0].cross(bAxes[0]),
                aAxes[0].cross(bAxes[1]),
                aAxes[0].cross(bAxes[2]),
                aAxes[1].cross(bAxes[0]),
                aAxes[1].cross(bAxes[1]),
                aAxes[1].cross(bAxes[2]),
                aAxes[2].cross(bAxes[0]),
                aAxes[2].cross(bAxes[1]),
                aAxes[2].cross(bAxes[2])
            };

            for (int i = 0; i < 15; i += 1) {
                if (MagnitudeSq(axesToTest[i]) < 0.001f)
                    continue;

                float depth = PenetrationDepth(a, b, axesToTest[i], &shouldFlip);
                if (depth <= 0.0f) {
                    return result;
                } else if (depth <result.depth) {
                    if (shouldFlip)
                        axesToTest[i] = axesToTest[i] * -1.0f;

                    result.depth = depth;
                    hitNormal = &axesToTest[i];
                }
            }

            if (hitNormal == 0)
                return result;

            EngineMath::Vector3 axis = *hitNormal;
            axis.normalize();
            std::vector<EngineMath::Vector3> c1 = ClipEdgesToOBB(GetEdges(b), a);
            std::vector<EngineMath::Vector3> c2 = ClipEdgesToOBB(GetEdges(a), b);

            result.contacts.reserve(c1.size() + c2.size());
            result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
            result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

            EngineMath::interval_t i = getInterval(a, axis);
            float distance = (i.max - i.min)* 0.5f - result.depth * 0.5f;
            EngineMath::Vector3 pointOnPlane = a->_position + axis * distance;

            for (int i = result.contacts.size() - 1; i >= 0; i -= 1) {
                EngineMath::Vector3 contact = result.contacts[i];
                result.contacts[i] = contact + (axis * (axis.dot(pointOnPlane - contact)));

                for (int j = result.contacts.size() - 1; j > i; j -= 1) {
                    if (MagnitudeSq(result.contacts[j] - result.contacts[i]) < 0.0001f) {
                        result.contacts.erase(result.contacts.begin() + j);
                        break;
                    }
                }
            }

            result.colliding = true;
            result.normal = axis;

            return result;
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

        void display(Scene &scene) {
            for (unsigned long int i = 0; i < scene.gameObjects.size(); i++) {
                transform_t *transform = scene.gameObjects[i]->getComponent<transform_t*>(Component::TRANSFORM);
                display(transform);
            }
        }

        /*
        ** COLLIDER PART
        */

       bool doesColliderHaveVolume(collider_t *collider) {
            if (collider->_colliderType == SPHERE || collider->_colliderType == SPHERE || collider->_colliderType == SPHERE)
                return true;
            return false;
       }

       CollisionManifold FindCollisionFeatures(collider_t *a, collider_t *b) {
            CollisionManifold result;
            ResetCollisionManifold(&result);

            if (a->_colliderType == SPHERE && b->_colliderType == SPHERE) {
                result = FindCollisionFeatures(reinterpret_cast<sphereCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));
            } else if (a->_colliderType == SPHERE && b->_colliderType == OBB) {
                result = FindCollisionFeatures(reinterpret_cast<OBBCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));
                result.normal = result.normal * -1.0f;
            } else if (a->_colliderType == SPHERE && b->_colliderType == AABB) {
                result = FindCollisionFeatures(reinterpret_cast<AABBCollider_t*>(b->_colliderData), reinterpret_cast<sphereCollider_t*>(a->_colliderData));
                result.normal = result.normal * -1.0f;
            } else if (a->_colliderType == OBB && b->_colliderType == OBB) {
                result = FindCollisionFeatures(reinterpret_cast<OBBCollider_t*>(a->_colliderData), reinterpret_cast<OBBCollider_t*>(b->_colliderData));
            } else if (a->_colliderType == OBB && b->_colliderType == SPHERE) {
                result = FindCollisionFeatures(reinterpret_cast<OBBCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));
            } else if (a->_colliderType == OBB && b->_colliderType == AABB) {
                result = FindCollisionFeatures(reinterpret_cast<AABBCollider_t*>(b->_colliderData), reinterpret_cast<OBBCollider_t*>(a->_colliderData));
                result.normal = result.normal * -1.0f;
            } else if (a->_colliderType == AABB && b->_colliderType == AABB) {
                result = FindCollisionFeatures(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<AABBCollider_t*>(b->_colliderData));
            } else if (a->_colliderType == AABB && b->_colliderType == OBB) {
                result = FindCollisionFeatures(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<OBBCollider_t*>(b->_colliderData));
            } else if (a->_colliderType == AABB && b->_colliderType == SPHERE) {
                result = FindCollisionFeatures(reinterpret_cast<AABBCollider_t*>(a->_colliderData), reinterpret_cast<sphereCollider_t*>(b->_colliderData));
            }

            return result;
       }

        void applyImpulseToCollider(collider_t *a, collider_t *b, const CollisionManifold collisionManifold, int c, bool isConstraint) {
            (void) c;
            float invMassA = invertMass(a);
            float invMassB = invertMass(b);
            float invMassSum = invMassA + invMassB;

            if (invMassSum == 0.0f) 
                return;

            EngineMath::Vector3 relativeVel = b->_velocity - a->_velocity;
            EngineMath::Vector3 relativeNorm = collisionManifold.normal;

            relativeNorm.normalize();

            if (relativeVel.dot(relativeNorm) > 0.0f)
                return;

            float e = fminf(a->_cor, b->_cor);
            float numerator = (-(1.0f + e) * relativeVel.dot(relativeNorm));
            float j = numerator / invMassSum;

            if (collisionManifold.contacts.size() > 0.0f && j != 0.0f)
                j /= static_cast<float>(collisionManifold.contacts.size());

            EngineMath::Vector3 impulse = relativeNorm * j;
            a->_velocity = a->_velocity - impulse * invMassA;
            if (!isConstraint)
                b->_velocity = b->_velocity + impulse * invMassB;

            EngineMath::Vector3 t = relativeVel - (relativeNorm * relativeVel.dot(relativeNorm));

            if (CMP(MagnitudeSq(t), 0.0f))
                return;

            t.normalize();
            numerator = -relativeVel.dot(t);

            float jt = numerator / invMassSum;

            if (collisionManifold.contacts.size() > 0.0f && jt != 0.0f)
                jt /= static_cast<float>(collisionManifold.contacts.size());

            if (CMP(jt, 0.0f))
                return;

            float friction = sqrtf(a->_friction * a->_friction);
            if (jt> j * friction)
                jt = j * friction;
            else if (jt< -j * friction)
                jt = -j * friction;

            EngineMath::Vector3 tangentImpuse = t * jt;

            a->_velocity = a->_velocity - tangentImpuse * invMassA;
            if (!isConstraint)
                b->_velocity = b->_velocity + tangentImpuse * invMassB;
        }

        void synchCollisionVolumes(collider_t *collider) {
            if (int(collider->_colliderType) == int(Collider::SPHERE)) {
                reinterpret_cast<sphereCollider_t*>(collider->_colliderData)->_position = collider->_position;
            }
            if (int(collider->_colliderType) == int(Collider::OBB)) {
                reinterpret_cast<OBBCollider_t*>(collider->_colliderData)->_position = collider->_position;
            }
            if (int(collider->_colliderType) == int(Collider::AABB)) {
                reinterpret_cast<AABBCollider_t*>(collider->_colliderData)->_position = collider->_position;
            }
        }

        float invertMass(collider_t *collider) {
            return (collider->_mass == 0.0f) ? 0.0f : 1.0f / collider->_mass;
        }

        void AddLinearImpulse(collider_t *collider, const EngineMath::Vector3& impulse) {
            collider->_velocity += impulse;
        }

        void updateVolumeCollider(collider_t *collider, float deltaTime) {
            const float damping = 0.98f;
            EngineMath::Vector3 acceleration = collider->_forces * invertMass(collider);
            collider->_velocity = collider->_velocity + collider->_inputVelocity;
            collider->_velocity = collider->_velocity + acceleration * deltaTime;
            collider->_velocity = collider->_velocity * damping;
            collider->_position = collider->_position + collider->_velocity * deltaTime;
            collider->_inputVelocity.x = 0;
            collider->_inputVelocity.y = 0;
            collider->_inputVelocity.z = 0;
            synchCollisionVolumes(collider);
        }

        void applyForcesToVolumeCollider(collider_t *collider) {
            collider->_forces = GRAVITY_CONST * collider->_mass;
        }

        void updateCollider(collider_t *collider, float deltaTime) {
            if (collider->_colliderType == SPHERE || collider->_colliderType == AABB || collider->_colliderType == OBB)
                updateVolumeCollider(collider, deltaTime);
        }

        void applyForcesToCollider(collider_t *collider) {
            if (collider->_colliderType == SPHERE || collider->_colliderType == AABB || collider->_colliderType == OBB)
                applyForcesToVolumeCollider(collider);
        }

        void solveConstraints(collider_t *collider, const std::vector<collider_t *> constraints) {
            if (!doesColliderHaveVolume(collider))
                return;
            for (unsigned int i = 0; i < constraints.size(); i += 1) {
                if (!doesColliderHaveVolume(constraints[i]))
                    continue;
                CollisionManifold manifold = FindCollisionFeatures(collider, constraints[i]);
                if (!manifold.colliding)
                    continue;
                for (unsigned int j = 0; j < manifold.contacts.size(); j += 1) {
                    applyImpulseToCollider(collider, constraints[i], manifold, j, true);
                }
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

        void resetUpdate(Scene &scene) {
            _colliders1.clear();
            _colliders2.clear();
            _results.clear();
        }

        float recoverFloatDelta(Scene &scene) {
            clock_t deltaClock = scene.getTimeDiff();
            unsigned long millis = deltaClock * 1000 / CLOCKS_PER_SEC;
            return static_cast<float>(millis) / 1000.0f;
        }

        void registerCollisions(Scene &scene) {
            for (unsigned long int i = 0; i < scene.gameObjects.size(); i += 1) {
                for (unsigned long int j = i + 1; j < scene.gameObjects.size(); j += 1) {
                    CollisionManifold result;
                    ResetCollisionManifold(&result);
                    collider_t* colliderA = scene.gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);
                    collider_t* colliderB = scene.gameObjects[j]->getComponent<collider_t *>(Component::COLLIDER);
                    
                    if (colliderA == nullptr || colliderB == nullptr)
                    continue;

                    if (doesColliderHaveVolume(colliderA) && doesColliderHaveVolume(colliderB)) {
                        result = FindCollisionFeatures(colliderA, colliderB);

                        if (result.colliding) {
                            _colliders1.push_back(colliderA);
                            _colliders2.push_back(colliderB);
                            _results.push_back(result);
                        }
                    }
                }
            }
        }

        void updateApplyForces(Scene &scene) {
            for (unsigned long int i = 0; i < scene.gameObjects.size(); i += 1) {
                collider_t* collider = scene.gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);
                if (collider == nullptr)
                    continue;

                applyForcesToCollider(collider);
            }
        }

        void updateApplyImpulse() {
            for (int k = 0; k < _impulseIteration; k += 1) {
                for (unsigned int i = 0; i < _results.size(); i += 1) {
                    for (unsigned int j = 0; j < _results[i].contacts.size(); j += 1) {
                        if (doesColliderHaveVolume(_colliders1[i]) && doesColliderHaveVolume(_colliders2[i])) {
                            applyImpulseToCollider(_colliders1[i], _colliders2[i], _results[i], j, false);
                        }
                    }
                }
            }
        }

        void updateAllColliders(Scene &scene) {
            float delta = recoverFloatDelta(scene);

            for (unsigned long int i = 0; i < scene.gameObjects.size(); i += 1) {
                collider_t* collider = scene.gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);
                if (collider == nullptr)
                    continue;
                updateCollider(collider, delta);
            }
        }

        void handleSinking() {
            for (unsigned int i = 0; i < _results.size(); i += 1) {
                if (doesColliderHaveVolume(_colliders1[i]) && doesColliderHaveVolume(_colliders2[i])) {
                    float totalMass = invertMass(_colliders1[i]) + invertMass(_colliders2[i]);
                    if (totalMass == 0.0f)
                            continue;
                    float depth = fmaxf(_results[i].depth - _penetrationSlack, 0.0f);
                    float scalar = depth / totalMass;
                    EngineMath::Vector3 correction = _results[i].normal * scalar * _linearProjectionPercent;

                    _colliders1[i]->_position = _colliders1[i]->_position - correction * invertMass(_colliders1[i]);
                    _colliders2[i]->_position = _colliders2[i]->_position + correction * invertMass(_colliders2[i]);

                    synchCollisionVolumes(_colliders1[i]);
                    synchCollisionVolumes(_colliders2[i]);
                }
            }

            
        }

        void updateSolveConstraints(Scene &scene) {
            for (unsigned long int i = 0; i < scene.gameObjects.size(); i += 1) {
                collider_t* collider = scene.gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);
                if (collider == nullptr)
                    continue;

                solveConstraints(collider, scene.getConstraints());
            }
        }

        void update(Scene &scene) override {
            // ArcLogger::trace("PhysicsModule::update");

            resetUpdate(scene);
            registerCollisions(scene);
            updateApplyForces(scene);
            updateApplyImpulse();
            // updateSolveConstraints(scene); // A LA FIN ?
            updateAllColliders(scene);
            handleSinking();
        }

        // void update(Scene &scene) override {
        //     float delta = 1;
        //     ArcLogger::trace("PhysicsModule::update");
        //     for (unsigned long int i = 0; i < scene.gameObjects.size(); i++) {
        //         transform_t* transform = scene.gameObjects[i]->getComponent<transform_t *>(Component::TRANSFORM);
        //         collider_t* collider = scene.gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);
        //         integrate(transform, delta);
        //         scene.gameObjects[i]->setComponent(Component::COLLIDER, new collider_t(Collider::SPHERE, new sphereCollider_t(EngineMath::Vector3(transform->_position), 1.1)));
        //         display(transform);
        //     }
        //     for (unsigned long int i = 0; i < scene.gameObjects.size(); i++) {
        //         collider_t *colliderA = scene.gameObjects[i]->getComponent<collider_t *>(Component::COLLIDER);

        //         for (unsigned long int j = i + 1; j < scene.gameObjects.size(); j++) {
        //             collider_t *colliderB = scene.gameObjects[j]->getComponent<collider_t *>(Component::COLLIDER);

        //             Intersection inter = intersect(colliderA, colliderB);
        //             if (inter.collided) {
        //                 transform_t *tr = scene.gameObjects[i]->getComponent<transform_t *>(Component::TRANSFORM);
        //                 transform_t *trj = scene.gameObjects[j]->getComponent<transform_t *>(Component::TRANSFORM);

        //                 std::cout << "COLLIDED" <<   inter.distance << std::endl;
        //                 scene.gameObjects[i]->setComponent(Component::TRANSFORM, new transform_t(EngineMath::Vector3(
        //                     tr->_position.x + inter.distance * (tr->_velocity.x ? (tr->_velocity.x < 0 ? -1 : 1) : 0),
        //                     tr->_position.y + inter.distance * (tr->_velocity.y ? (tr->_velocity.y < 0 ? -1 : 1) : 0),
        //                     tr->_position.z + inter.distance * (tr->_velocity.z ? (tr->_velocity.z < 0 ? -1 : 1) : 0)),
        //                     tr->_velocity * -1,
        //                     tr->_acceleration * -1));
        //                 scene.gameObjects[j]->setComponent(Component::TRANSFORM, new transform_t(EngineMath::Vector3(
        //                     trj->_position.x + inter.distance * (trj->_velocity.x ? (trj->_velocity.x < 0 ? -1 : 1) : 0),
        //                     trj->_position.y + inter.distance * (trj->_velocity.y ? (trj->_velocity.y < 0 ? -1 : 1) : 0),
        //                     trj->_position.z + inter.distance * (trj->_velocity.z ? (trj->_velocity.z < 0 ? -1 : 1) : 0)),
        //                     trj->_velocity * -1,
        //                     trj->_acceleration * -1));
        //             }
        //         }
        //     }
        // }

        void testModule() {
            transform_t *tr = new transform_t(EngineMath::Vector3(), EngineMath::Vector3(5,5,5), EngineMath::Vector3(-0.5, -0.5, -0.5));

            sphereCollider_t *sp1 = new sphereCollider_t(EngineMath::Vector3(0.0f,0.0f,0.0f), 1.0f);
            sphereCollider_t *sp2 = new sphereCollider_t(EngineMath::Vector3(2.0f,0.0f,0.0f), 1.0f);
            sphereCollider_t *sp3 = new sphereCollider_t(EngineMath::Vector3(-2.0f,-0.0f,-0.0f), 1.5f);
            // sphereCollider_t *sp4 = new sphereCollider_t(EngineMath::Vector3(-1.0f,5.0f,5.0f), 1.1f);
            sphereCollider_t *sp5 = new sphereCollider_t(EngineMath::Vector3(2.0f, 2.0f, 2.0f), 1.0f);
            sphereCollider_t *sp6 = new sphereCollider_t(EngineMath::Vector3(1.5f, 1.5f, 1.5f), 1.0f);

            // AABBCollider_t *aa1 = new AABBCollider_t (EngineMath::Vector3(0.0f,0.0f,0.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));
            // AABBCollider_t *aa2 = new AABBCollider_t (EngineMath::Vector3(1.0f, 1.0f, 1.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));
            // AABBCollider_t *aa3 = new AABBCollider_t (EngineMath::Vector3(0.5f, 0.5f, 0.5f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));
            // AABBCollider_t *aa4 = new AABBCollider_t (EngineMath::Vector3(2.0f, 2.0f, 2.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f));

            OBBCollider_t *obb1 = new OBBCollider_t (EngineMath::Vector3(0.0f, 1.0f, 0.0f), EngineMath::Vector3(0.5f, 1.0f, 0.5f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));
            OBBCollider_t *obb2 = new OBBCollider_t (EngineMath::Vector3(0.4f, 0.0f, 0.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f), EngineMath::m3_t(0, 1, 0, 1, 0, 0, 0, 0, 1));
            OBBCollider_t *obb3 = new OBBCollider_t (EngineMath::Vector3(1.0f, 0.0f, 0.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f), EngineMath::m3_t(0, 1, 0, 1, 0, 0, 0, 0, 1));
            OBBCollider_t *obb4 = new OBBCollider_t (EngineMath::Vector3(2.0f, 0.0f, 0.0f), EngineMath::Vector3(pow(2.0f, 0.5) / 2, pow(2.0f, 0.5) / 2, 0.5f), EngineMath::m3_t(1, 1, 0, -1, 1, 0, 0, 0, 1));

            OBBCollider_t *obb5 = new OBBCollider_t (EngineMath::Vector3(1.5f, 0.0f, 0.0f), EngineMath::Vector3(0.5f, 1.0f, 0.5f), EngineMath::m3_t(0, 1, 0, -1, 0, 0, 0, 0, 1));

            OBBCollider_t *obb6 = new OBBCollider_t (EngineMath::Vector3(2.0f, 0.0f, 0.0f), EngineMath::Vector3(pow(2.0f, 0.5) / 2, pow(18.0f, 0.5) / 2, 0.5f), EngineMath::m3_t(1, 1, 0, -3, 3, 0, 0, 0, 1));
            OBBCollider_t *obb7 = new OBBCollider_t (EngineMath::Vector3(2.0f, 0.0f, 0.0f), EngineMath::Vector3(pow(18.0f, 0.5) / 2, pow(2.0f, 0.5) / 2, 0.5f), EngineMath::m3_t(3, 3, 0, -1, 1, 0, 0, 0, 1));
            OBBCollider_t *obb8 = new OBBCollider_t (EngineMath::Vector3(2.0f, 2.0f, 2.0f), EngineMath::Vector3(0.5f, 1.0f, 0.5f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));
            OBBCollider_t *obb9 = new OBBCollider_t (EngineMath::Vector3(2.0f, 0.0f, 0.0f), EngineMath::Vector3(0.5f, 0.5f, 0.5f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));
            OBBCollider_t *obb10 = new OBBCollider_t (EngineMath::Vector3(2.0f, 0.0f, 0.0f), EngineMath::Vector3(0.5f, 1.5f, 1.5f), EngineMath::m3_t(1, 0, 0, 0, 1, 0, 0, 0, 1));

            rayCollider_t *ray1 = new rayCollider_t (EngineMath::Vector3(0.0f, 0.0f, 0.0f), EngineMath::Vector3(1.0f, 1.0f, 1.0f));


            Intersection inter1 = intersect(sp1, sp2);
            Intersection inter2 = intersect(sp1, sp3);

            Intersection inter3 = intersect(obb1, obb2);
            Intersection inter4 = intersect(obb1, obb3);
            Intersection inter5 = intersect(obb1, obb4);
            Intersection inter6 = intersect(obb1, obb5);
            Intersection inter7 = intersect(obb1, obb6);
            Intersection inter8 = intersect(obb1, obb7);

            Intersection inter9 = intersect(ray1, sp2);
            Intersection inter10 = intersect(ray1, sp5);
            Intersection inter11 = intersect(ray1, sp6);

            Intersection inter12 = intersect(ray1, obb8);
            Intersection inter13 = intersect(ray1, obb9);
            Intersection inter14 = intersect(ray1, obb10);

            // Intersection inter3 = intersect(aa1, aa2);
            // Intersection inter4 = intersect(aa1, aa3);
            // Intersection inter5 = intersect(aa1, aa4);


            ASSERT(inter1.collided == false);
            ASSERT(inter1.distance == 0.00f);

            ASSERT(inter2.collided == true);
            ASSERT(inter2.distance == -0.5f);

            ASSERT(inter3.collided == true);
            ASSERT(inter4.collided == true);
            ASSERT(inter5.collided == false);
            ASSERT(inter6.collided == true);
            ASSERT(inter7.collided == true);
            ASSERT(inter8.collided == false);

            ASSERT(inter9.collided == false);
            ASSERT(inter10.collided == true);
            ASSERT(inter11.collided == true);

            ASSERT(inter12.collided == true);
            ASSERT(inter13.collided == false);
            ASSERT(inter14.collided == true);

            // ASSERT(inter3.collided == true);
            // ASSERT(inter4.collided == true);
            // ASSERT(inter5.collided == false);


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
        std::vector<collider_t*> _colliders1;
        std::vector<collider_t*> _colliders2;
        std::vector<CollisionManifold> _results;
        float _linearProjectionPercent;
        float _penetrationSlack;
        int _impulseIteration;
};