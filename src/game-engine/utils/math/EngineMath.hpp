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

        float dotSquare() const {
            return (x * x + y * y + z * z);
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
        Vector3 cross(const Vector3 &other) const {
            return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
        }
        float length() {return pow(x * x + y * y + z * z, 0.5);}
        float x;
        float y;
        float z;
    };

    typedef struct m4_s {
        union {
            struct {
                float _00, _01, _02, _03,
                    _10, _11, _12, _13,
                    _20, _21, _22, _23;
            };
            float _matrix[16] = { 0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0 };
        };

        float* operator[](int i) {
            return &(_matrix[i * 4]);
        };
    } m4_t;

    typedef struct m3_s {
        union {
            struct {
                float _00, _01, _02,
                    _10, _11, _12,
                    _20, _21, _22;
            };
            float _matrix[9] = { 0, 0, 0,
                                  0, 0, 0,
                                  0, 0, 0 };
        };

        float* operator[](int i) {
            return &(_matrix[i * 3]);
        };
    } m3_t;

    void Transpose(const float *srcMat, float *dstMat, 
   int srcRows, int srcCols);
    m3_t Transpose(const m3_t& matrix);
    m4_t Transpose(const m4_t& matrix);
    void Transpose(const float *srcMat, float *dstMat, int srcRows, int srcCols) {
        for (int i = 0; i < srcRows * srcCols; i++) {
            int row = i / srcRows;
            int col = i % srcRows;
            dstMat[i] = srcMat[srcCols * col + row];
        };
    }

    m3_t Transpose(const m3_t& matrix) {
        m3_t result;
        Transpose(matrix._matrix, result._matrix, 3, 3);
        return result;
    }

    m4_t Transpose(const m4_t& matrix) {
        m4_t result;
        Transpose(matrix._matrix, result._matrix, 4, 4);
        return result;
    }
}


#endif