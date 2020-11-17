#ifndef ENGINE_MATH_
#define ENGINE_MATH_

#include <map>
#include <math.h>
#include <cmath>
#include <cfloat>
#include <iomanip>

namespace EngineMath {
    #define CMP(x, y)                    \
    (fabsf((x)-(y)) <= FLT_EPSILON * \
        fmaxf(1.0f,                    \
        fmaxf(fabsf(x), fabsf(y)))     \
    )
    #define DEG2RAD(x) ((x) * 0.0174533f)
    #define cot(x) 1/tan(x)
    struct Vector2 {
        float x;
        float y;
        Vector2(): x(0), y(0) {};
        Vector2(float x, float y): x(x), y(y) {};

    };
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
        bool operator==(const Vector3 &other) {
            return (x == other.x && y == other.y && z == other.z);
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
        Vector3 operator-() {
            x = -x;
            y = -y;
            z = -z;
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
        float operator[](int i) {
            float array[3] = { x, y, z };

            return array[i];
        };
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
    
    float Magnitude(const Vector3& v) {
        return sqrtf(v.dot(v));
    }
    
    float MagnitudeSq(const Vector3& v) {
        return v.dot(v);
    }
    
    typedef struct interval_s {
        float min;
        float max;
    } interval_t;

    typedef struct Plane {
        Vector3 normal;
        float distance;

        inline Plane() : normal(1, 0, 0) { }
        inline Plane(const Vector3& n, float d) :
            normal(n), distance(d) { }
    } Plane;

    typedef struct m2_s {
        union {
            struct {
                float _00, _01,
                    _10, _11;
            };
            float _matrix[4];
        };
        inline m2_s() { 
            _00 = _11 = 1.0f;
            _01 = _10 = 0.0f;
        }
        inline m2_s(float a00, float a01, float a10, float a11) {
            _00 = a00;
            _01 = a01;
            _10 = a10;
            _11 = a11;
        }
        inline float* operator[](int i) {
            return &(_matrix[i * 2]);
        }
    } m2_t;

    typedef struct m4_s {
        union {
            struct {
                float _00, _01, _02, _03,
                    _10, _11, _12, _13,
                    _20, _21, _22, _23,
                    _30, _31, _32, _33;
            };
            float _matrix[16] = { 0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0 };
        };
        inline m4_s() {
            _00 = _11 = _22 = _33 = 1.0f;
            _01 = _02 = _03 = _10 = 0.0f;
            _12 = _13 = _20 = _21 = 0.0f;
            _23 = _30 = _31 = _32 = 0.0f;
        }
        inline m4_s(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33) {
            _00 = a00;
            _01 = a01;
            _02 = a02;
            _03 = a03;
            _10 = a10;
            _11 = a11;
            _12 = a12;
            _13 = a13;
            _20 = a20;
            _21 = a21;
            _22 = a22;
            _23 = a23;
            _30 = a30;
            _31 = a31;
            _32 = a32;
            _33 = a33;
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

        inline m3_s() {
            _00 = _11 = _22 = 1.0f;
            _01 = _02 = _10 = 0.0f;
            _12 = _20 = _21 = 0.0f;
        };

        inline m3_s(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22) {
            _00 = a00;
            _11 = a11;
            _22 = a22;
            _01 = a01;
            _02 = a02;
            _10 = a10;
            _12 = a12;
            _20 = a20;
            _21 = a21;
        };

        float* operator[](int i) {
            return &(_matrix[i * 3]);
        };
    } m3_t;


    void Transpose(const float *srcMat, float *dstMat, 
    int srcRows, int srcCols);
    m3_t Transpose(const m3_t& matrix);
    m4_t Transpose(const m4_t& matrix);
    m3_t Cut(const m4_t& mat, int row, int col);
    m4_t Minor(const m4_t& mat);
    m4_t Cofactor(const m4_t& mat);
    float Determinant(const m4_t& mat);
    float Determinant(const m3_t& mat);

    void Transpose(const float *srcMat, float *dstMat, int srcRows, int srcCols) {
        for (int i = 0; i < srcRows * srcCols; i++) {
            int row = i / srcRows;
            int col = i % srcRows;
            dstMat[i] = srcMat[srcCols * col + row];
        };
    }
    m2_t Transpose(const m2_t& matrix) {
        m2_t result;
        Transpose(matrix._matrix, result._matrix, 2, 2);
        return result;
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
    
    m2_t operator*(const m2_t& matrix, float scalar) {
        m2_t result;
        for (int i = 0; i < 4; ++i) {
            result._matrix[i] = matrix._matrix[i] * scalar;
        }
        return result;
    }
    
    m3_t operator*(const m3_t& matrix, float scalar) {
        m3_t result;
        for (int i = 0; i < 9; ++i) {
            result._matrix[i] = matrix._matrix[i] * scalar;
        }
        return result;
    }

    m4_t operator*(const m4_t& matrix, float scalar) {
        m4_t result;
        for (int i = 0; i < 16; ++i) {
            result._matrix[i] = matrix._matrix[i] * scalar;
        }
        return result;
    }

    bool Multiply(float* out, const float* matA, int aRows, 
    int aCols, const float* matB, int bRows, int bCols) {
    if (aCols != bRows) { 
        return false; 
    }
    for (int i = 0; i < aRows; ++i) {
        for (int j = 0; j < bCols; ++j) {
            out[bCols * i + j] = 0.0f;
            for (int k = 0; k < bRows; ++k) {
                int a = aCols * i + k;
                int b = bCols * k + j;
                out[bCols * i + j] += matA[a] * matB[b];
            }
        }
    }
    return true;
    }

    m2_t operator*(const m2_t& matA, const m2_t& matB) {
        m2_t res;
        Multiply(res._matrix, matA._matrix, 
        2, 2, matB._matrix, 2, 2);
        return res;
    }

    m3_t operator*(const m3_t& matA, const m3_t& matB) {
        m3_t res;
        Multiply(res._matrix, matA._matrix, 
        3, 3, matB._matrix, 3, 3);
        return res;
    }

    m4_t operator*(const m4_t& matA, const m4_t& matB) {
        m4_t res;
        Multiply(res._matrix, matA._matrix, 
        4, 4, matB._matrix, 4, 4);
        return res;
    }

    float Determinant(const m2_t& matrix) {
        return matrix._00 * matrix._11 - matrix._01 * matrix._10;
    }

    m2_t Cut(const m3_t& mat, int row, int col) {
        m2_t result;
        int index = 0;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (i == row || j == col) {
                    continue;
                }
                int target = index++;
                int source = 3 * i + j;
                result._matrix[target] = mat._matrix[source];
            }
        }
        return result;
    }

    m3_t Cut(const m4_t& mat, int row, int col) {
        m3_t result;
        int index = 0;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (i == row || j == col) {
                    continue;
                }
                int target = index++;
                int source = 4 * i + j;
                result._matrix[target] = mat._matrix[source];
            }
        }

        return result;
    }

    m4_t Minor(const m4_t& mat) {
        m4_t result;

        for (int i = 0; i <4; ++i) {
            for (int j = 0; j <4; ++j) {
                result[i][j] = Determinant(Cut(mat, i, j));
            }
        }

        return result;
    }

    m3_t Minor(const m3_t& mat) {
        m3_t result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result[i][j] = Determinant(Cut(mat, i, j));
            }
        }
        return result;
    }

    m2_t Minor(const m2_t& mat) {
        return m2_t(
            mat._11, mat._10,
            mat._01, mat._00
        );
    }

    void Cofactor(float* out, const float* minor, int rows, int cols) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int t = cols * j + i;
                int s = cols * j + i;
                float sign = powf(-1.0f, i + j);
                out[t] = minor[s] * sign;
            }
        }
    }
    m4_t Cofactor(const m4_t& mat) {
        m4_t result;
        Cofactor(result._matrix, Minor(mat)._matrix, 4, 4);
        return result;
    }

    m2_t Cofactor(const m2_t& mat) {
        m2_t result;
        Cofactor(result._matrix, Minor(mat)._matrix, 2, 2);
        return result;
    }

    m3_t Cofactor(const m3_t& mat) {
        m3_t result;
        Cofactor(result._matrix, Minor(mat)._matrix, 3, 3);
        return result;
    }

    float Determinant(const m3_t& mat) {
        float result = 0.0f;
        m3_t cofactor = Cofactor(mat);
        for (int j = 0; j < 3; ++j) {
            int index = 3 * 0 + j;
            result += mat._matrix[index] * cofactor[0][j];
        }
        return result;
    }

    float Determinant(const m4_t& mat) {
        float result = 0.0f;

        m4_t cofactor = Cofactor(mat);
        for (int j = 0; j < 4; ++j) {
            result += mat._matrix[4 * 0 + j] * cofactor[0][j];
        }

        return result;
    }
    m2_t Adjugate(const m2_t& mat) {
        return Transpose(Cofactor(mat));
    }
    m3_t Adjugate(const m3_t& mat) {
        return Transpose(Cofactor(mat));
    }
    m4_t Adjugate(const m4_t& mat) {
        return Transpose(Cofactor(mat));
    }
    m2_t Inverse(const m2_t& mat) {
        float det = Determinant(mat);
        if (CMP(det, 0.0f)) { return m2_t(); }
        return Adjugate(mat) * (1.0f / det);
    }
    m3_t Inverse(const m3_t& mat) {
        float det = Determinant(mat);
        if (CMP(det, 0.0f)) { return m3_t(); }
        return Adjugate(mat) * (1.0f / det);
    }
    // m4_t Inverse(const m4_t& mat) {
    //     float det = Determinant(mat);
    //     if (CMP(det, 0.0f)) { return m4_t(); }
    //     return Adjugate(mat) * (1.0f / det);
    // }
    m4_t Inverse(const m4_t& m) {
       float det 
            = m._00 * m._11 * m._22 * m._33 + m._00 * m._12 * m._23 * m._31 + m._00 * m._13 * m._21 * m._32
            + m._01 * m._10 * m._23 * m._32 + m._01 * m._12 * m._20 * m._33 + m._01 * m._13 * m._22 * m._30
            + m._02 * m._10 * m._21 * m._33 + m._02 * m._11 * m._23 * m._30 + m._02 * m._13 * m._20 * m._31
            + m._03 * m._10 * m._22 * m._31 + m._03 * m._11 * m._20 * m._32 + m._03 * m._12 * m._21 * m._30
            - m._00 * m._11 * m._23 * m._32 - m._00 * m._12 * m._21 * m._33 - m._00 * m._13 * m._22 * m._31
            - m._01 * m._10 * m._22 * m._33 - m._01 * m._12 * m._23 * m._30 - m._01 * m._13 * m._20 * m._32
            - m._02 * m._10 * m._23 * m._31 - m._02 * m._11 * m._20 * m._33 - m._02 * m._13 * m._21 * m._30
            - m._03 * m._10 * m._21 * m._32 - m._03 * m._11 * m._22 * m._30 - m._03 * m._12 * m._20 * m._31;

        if (CMP(det, 0.0f)) { 
            return m4_t(); 
        }
        float i_det = 1.0f / det;

        m4_t result;
        result._00 = (m._11 * m._22 * m._33 + m._12 * m._23 * m._31 + m._13 * m._21 * m._32 - m._11 * m._23 * m._32 - m._12 * m._21 * m._33 - m._13 * m._22 * m._31) * i_det;
        result._01 = (m._01 * m._23 * m._32 + m._02 * m._21 * m._33 + m._03 * m._22 * m._31 - m._01 * m._22 * m._33 - m._02 * m._23 * m._31 - m._03 * m._21 * m._32) * i_det;
        result._02 = (m._01 * m._12 * m._33 + m._02 * m._13 * m._31 + m._03 * m._11 * m._32 - m._01 * m._13 * m._32 - m._02 * m._11 * m._33 - m._03 * m._12 * m._31) * i_det;
        result._03 = (m._01 * m._13 * m._22 + m._02 * m._11 * m._23 + m._03 * m._12 * m._21 - m._01 * m._12 * m._23 - m._02 * m._13 * m._21 - m._03 * m._11 * m._22) * i_det;
        result._10 = (m._10 * m._23 * m._32 + m._12 * m._20 * m._33 + m._13 * m._22 * m._30 - m._10 * m._22 * m._33 - m._12 * m._23 * m._30 - m._13 * m._20 * m._32) * i_det;
        result._11 = (m._00 * m._22 * m._33 + m._02 * m._23 * m._30 + m._03 * m._20 * m._32 - m._00 * m._23 * m._32 - m._02 * m._20 * m._33 - m._03 * m._22 * m._30) * i_det;
        result._12 = (m._00 * m._13 * m._32 + m._02 * m._10 * m._33 + m._03 * m._12 * m._30 - m._00 * m._12 * m._33 - m._02 * m._13 * m._30 - m._03 * m._10 * m._32) * i_det;
        result._13 = (m._00 * m._12 * m._23 + m._02 * m._13 * m._20 + m._03 * m._10 * m._22 - m._00 * m._13 * m._22 - m._02 * m._10 * m._23 - m._03 * m._12 * m._20) * i_det;
        result._20 = (m._10 * m._21 * m._33 + m._11 * m._23 * m._30 + m._13 * m._20 * m._31 - m._10 * m._23 * m._31 - m._11 * m._20 * m._33 - m._13 * m._21 * m._30) * i_det;
        result._21 = (m._00 * m._23 * m._31 + m._01 * m._20 * m._33 + m._03 * m._21 * m._30 - m._00 * m._21 * m._33 - m._01 * m._23 * m._30 - m._03 * m._20 * m._31) * i_det;
        result._22 = (m._00 * m._11 * m._33 + m._01 * m._13 * m._30 + m._03 * m._10 * m._31 - m._00 * m._13 * m._31 - m._01 * m._10 * m._33 - m._03 * m._11 * m._30) * i_det;
        result._23 = (m._00 * m._13 * m._21 + m._01 * m._10 * m._23 + m._03 * m._11 * m._20 - m._00 * m._11 * m._23 - m._01 * m._13 * m._20 - m._03 * m._10 * m._21) * i_det;
        result._30 = (m._10 * m._22 * m._31 + m._11 * m._20 * m._32 + m._12 * m._21 * m._30 - m._10 * m._21 * m._32 - m._11 * m._22 * m._30 - m._12 * m._20 * m._31) * i_det;
        result._31 = (m._00 * m._21 * m._32 + m._01 * m._22 * m._30 + m._02 * m._20 * m._31 - m._00 * m._22 * m._31 - m._01 * m._20 * m._32 - m._02 * m._21 * m._30) * i_det;
        result._32 = (m._00 * m._12 * m._31 + m._01 * m._10 * m._32 + m._02 * m._11 * m._30 - m._00 * m._11 * m._32 - m._01 * m._12 * m._30 - m._02 * m._10 * m._31) * i_det;
        result._33 = (m._00 * m._11 * m._22 + m._01 * m._12 * m._20 + m._02 * m._10 * m._21 - m._00 * m._12 * m._21 - m._01 * m._10 * m._22 - m._02 * m._11 * m._20) * i_det;

    #ifdef  DO_SANITY_TESTS
    #ifndef NO_EXTRAS
        if (result * m != m4_t()) {
            std::cout << "ERROR! Expecting matrix x inverse to equal identity!\n";
        }
    #endif 
    #endif

        return result;
    }
    m4_t Translation(float x, float y, float z) {
        return m4_t(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            x, y, z, 1.0f
        );
    }

    m4_t Translation(const Vector3& pos) {
        return m4_t(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            pos.x,pos.y,pos.z,1.0f
        );
    }

    Vector3 GetTranslation(const m4_t& mat) {
        return Vector3(mat._30, mat._31, mat._32);
    }

    m4_t Scale(float x, float y, float z) {
        return m4_t(
            x, 0.0f, 0.0f, 0.0f,
            0.0f, y, 0.0f, 0.0f,
            0.0f, 0.0f, z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    m4_t Scale(const Vector3&vec) {
        return m4_t(
            vec.x,  0.0f, 0.0f, 0.0f,
            0.0f, vec.y,0.0f, 0.0f,
            0.0f, 0.0f, vec.z,0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }
    Vector3 GetScale(const m4_t& mat) {
        return Vector3(mat._00, mat._11, mat._22);
    }

    m4_t XRotation(float angle) {
        angle = DEG2RAD(angle);
        return m4_t(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosf(angle), sinf(angle), 0.0f,
            0.0f, -sinf(angle), cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    m3_t XRotation3x3(float angle) {
        angle = DEG2RAD(angle);
        return m3_t(
            1.0f, 0.0f, 0.0f,
            0.0f, cosf(angle), sinf(angle),
            0.0f, -sinf(angle), cos(angle)
        );
    }

    m4_t YRotation(float angle) {
        angle = DEG2RAD(angle);
        return m4_t(
            cosf(angle), 0.0f, -sinf(angle), 0.0f,
            0.0f,         1.0f, 0.0f,         0.0f,
            sinf(angle), 0.0f, cosf(angle), 0.0f,
            0.0f,         0.0f, 0.0f,         1.0f
        );
    }

    m3_t YRotation3x3(float angle) {
        angle = DEG2RAD(angle);
        return m3_t(
            cosf(angle), 0.0f, -sinf(angle),
            0.0f,         1.0f, 0.0f,
            sinf(angle), 0.0f, cosf(angle)
        );
    }

    m4_t ZRotation(float angle) {
        angle = DEG2RAD(angle);
        return m4_t(
            cosf(angle), sinf(angle), 0.0f, 0.0f,
            -sinf(angle), cosf(angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    m3_t ZRotation3x3(float angle) {
        angle = DEG2RAD(angle);
        return m3_t(
            cosf(angle), sinf(angle), 0.0f,
            -sinf(angle), cosf(angle), 0.0f,
            0.0f, 0.0f, 1.0f
        );
    }


    m4_t Rotation(float pitch, float yaw, float roll) {
        return  ZRotation(roll) * 
                XRotation(pitch) * 
                YRotation(yaw);
    }
    m3_t Rotation3x3(float pitch, float yaw, float roll) {
        return  ZRotation3x3(roll) *
                XRotation3x3(pitch) * 
                YRotation3x3(yaw);
    }

    m4_t AxisAngle(const Vector3& axis, float angle) {
        angle = DEG2RAD(angle);
        float c = cosf(angle);
        float s = sinf(angle);
        float t = 1.0f - cosf(angle);

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;
        if (!CMP(MagnitudeSq(axis), 1.0f)) {
            float inv_len = 1.0f / Magnitude(axis);
            x *= inv_len; // Normalize x
            y *= inv_len; // Normalize y
            z *= inv_len; // Normalize z
        } // x, y, and z are a normalized vector

        return m4_t(
            t*(x*x) + c, t*x*y + s*z, t*x*z - s*y, 0.0f,
            t*x*y - s*z, t*(y*y) + c, t*y*z + s*x, 0.0f,
            t*x*z + s*y, t*y*z - s*x, t*(z*z) + c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    m3_t AxisAngle3x3(const Vector3& axis, float angle) {
        angle = DEG2RAD(angle);
        float c = cosf(angle);
        float s = sinf(angle);
        float t = 1.0f - cosf(angle);

        float x = axis.x;
        float y = axis.y;
        float z =axis.z;
        if (!CMP(MagnitudeSq(axis), 1.0f)) {
            float inv_len = 1.0f / Magnitude(axis);
            x *= inv_len; 
            y *= inv_len; 
            z *= inv_len;
        }

        return m3_t(
            t * (x * x) + c,t * x * y + s * z,t * x * z - s * y, 
            t * x * y - s * z,t * (y * y) + c,t * y * z + s * x, 
            t * x * z + s * y,t * y * z - s * x,t * (z * z) + c
        );
    }

    Vector3 MultiplyPoint(const Vector3& vec, const m4_t& mat) {
        Vector3 result;
        result.x = vec.x * mat._00 + vec.y * mat._10 + 
                    vec.z * mat._20 + 0.0f  * mat._30;
        result.y = vec.x * mat._01 + vec.y * mat._11 + 
                    vec.z * mat._21 + 0.0f  * mat._31;
        result.z = vec.x * mat._02 + vec.y * mat._12 + 
                    vec.z * mat._22 + 0.0f  * mat._32;
        return result;
    }
    
    Vector3 MultiplyVector(const Vector3& vec, const m4_t& mat) {
        Vector3 result;
        result.x = vec.x * mat._00 + vec.y * mat._10 + 
                    vec.z * mat._20 + 0.0f  * mat._30;
        result.y = vec.x * mat._01 + vec.y * mat._11 + 
                    vec.z * mat._21 + 0.0f  * mat._31;
        result.z = vec.x * mat._02 + vec.y * mat._12 + 
                    vec.z * mat._22 + 0.0f  * mat._32;
        return result;
    }

    Vector3 MultiplyVector(const Vector3& vec, const m3_t& mat) {
        Vector3 result;
        result.x = vec.dot(Vector3(mat._00, mat._10, mat._20));
        result.y = vec.dot(Vector3(mat._01, mat._11, mat._21));
        result.z = vec.dot(Vector3(mat._02, mat._12, mat._22));
        return result;
    }

    m4_t Transform(const Vector3& scale, const Vector3& eulerRotation,
        const Vector3& translate) {
            return Scale(scale) *
            Rotation(eulerRotation.x, 
                    eulerRotation.y, 
                    eulerRotation.z) *
            Translation(translate);
    }

    m4_t Transform(const Vector3& scale, const Vector3& rotationAxis,
        float rotationAngle, const Vector3& translate) {
            return Scale(scale) * 
            AxisAngle(rotationAxis, rotationAngle) * 
            Translation(translate);
    }
    m4_t LookAt(const Vector3& position, const Vector3& target, const Vector3& up) {
        Vector3 forward = target - position;
        forward.normalize();
        Vector3 right = up.cross(forward);
        right.normalize();
        Vector3 newUp = forward.cross(right);

        return m4_t(
            right.x, newUp.x, forward.x, 0.0f,
            right.y, newUp.y, forward.y, 0.0f,
            right.z, newUp.z, forward.z, 0.0f,
            -right.dot(position),
            -newUp.dot(position),
            -forward.dot(position), 1.0f
        );
    }

    m4_t Projection(float fov, float aspect, float zNear, float zFar) {
        float tanHalfFov = tanf(DEG2RAD((fov * 0.5f)));
        float fovY = 1.0f / tanHalfFov;
        float fovX = fovY / aspect;
        m4_t result;
        result._00 = fovX;
        result._11 = fovY;

        result._22 = zFar / (zFar - zNear); 
        result._23 = 1.0f;

        result._32 = -zNear * result._33; 
        result._33 = 0.0f;
        return result;
    }

    m4_t Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
        float _00 = 2.0f / (right - left);
        float _11 = 2.0f / (top - bottom);
        float _22 = 1.0f / (zFar - zNear);
        float _30 = (left + right) / (left - right);
        float _31 = (top + bottom) / (bottom - top);
        float _32 = (zNear) / (zNear - zFar);

        return m4_t(
            _00, 0.0f, 0.0f, 0.0f,
            0.0f,  _11, 0.0f, 0.0f,
            0.0f, 0.0f,  _22, 0.0f,
            _30,  _31,  _32, 1.0f
        ); 
    }

    void display(m3_t matrix) {
        std::cout << std::setprecision(4);
        std::cout << matrix._00 << "\t" << matrix._01 << "\t" << matrix._02 << std::endl;
        std::cout << matrix._10 << "\t" << matrix._11 << "\t" << matrix._12 << std::endl;
        std::cout << matrix._20 << "\t" << matrix._21 << "\t" << matrix._22 << std::endl;
    }

    void display(m4_t matrix) {
        std::cout << std::setprecision(4);
        std::cout << matrix._00 << "\t" << matrix._01 << "\t" << matrix._02 << "\t" << matrix._03 << std::endl;
        std::cout << matrix._10 << "\t" << matrix._11 << "\t" << matrix._12 << "\t" << matrix._13 << std::endl;
        std::cout << matrix._20 << "\t" << matrix._21 << "\t" << matrix._22 << "\t" << matrix._23 << std::endl;
        std::cout << matrix._30 << "\t" << matrix._31 << "\t" << matrix._32 << "\t" << matrix._33 << std::endl;   
    }
}

#endif