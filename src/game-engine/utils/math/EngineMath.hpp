#ifndef ENGINE_MATH_
#define ENGINE_MATH_

#include <map>
#include <math.h>

namespace EngineMath {
    class Vector3 {
    public:
        Vector3(): x(0), y(0), z(0) {};
        Vector3(float x, float y, float z): x(x), y(y), z(z) {};
        Vector3(const Vector3 &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        };
        Vector3 operator=(const Vector3 &other) {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }
        Vector3 operator+(const Vector3 &other) const {
            return {x + other.x, y + other.y, z + other.z};
        }
        Vector3 operator-=(const float &v) {
            x -= v;
            y -= v;
            z -= v;
            return *this;
        }
        Vector3 operator+=(const float &v) {
            x += v;
            y += v;
            z += v;
            return *this;
        }
        Vector3 operator+=(const Vector3 &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }
        Vector3 operator*=(const float &multiplier) {
            x *= multiplier;
            y *= multiplier;
            z *= multiplier;
            return *this;
        }
        float dot(const Vector3 &other) const {
            return (x * other.x + y * other.y + z * other.z);
        }
        Vector3 operator-(const Vector3 &other) const {
            return {x - other.x, y - other.y, z - other.z};
        }
        Vector3 operator/(const float divider) const {
            return {x / divider, y / divider, z / divider};
        }
        Vector3 operator*(const float multiplier) const {
            return {x * multiplier, y * multiplier, z * multiplier};
        }
        Vector3 max(const Vector3 &other) {
            return {x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z};
        };

        std::string getDisplayInfo() const {
            return ("x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z));
        }

        void normalize() {
            float vecLength = length();

            x /= vecLength;
            y /= vecLength;
            z /= vecLength;
        }

        float maxValue() {
            if (x > y && x > z)
                return x;
            if (y > z)
                return y;
            return z;
        }
        float distance (const Vector3 &other) const {
            return (other - *this).length();
        }
        Vector3 cross(const Vector3 &other) {
            return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
        }
        float length() {return pow(x * x + y * y + z * z, 0.5);}
        float x;
        float y;
        float z;
    };
}

#endif