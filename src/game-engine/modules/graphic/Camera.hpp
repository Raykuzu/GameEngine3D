#ifndef _H_CAMERA_
#define _H_CAMERA_

#include "EngineMath.hpp"
class Camera
{
protected:
    float m_nFov;
    float m_nAspect;
    float m_nNear;
    float m_nFar;
    float m_nWidth;
    float m_nHeight;
    EngineMath::m4_t m_matWorld; // World Transform
    // View Transform = Inverse(World Transform)
    EngineMath::m4_t m_matProj;
    int m_nProjectionMode;
    // ^ 0 - Perspective, 1 - Ortho, 2 - User
public:
    Camera()
    {
        m_nFov = 60.0f;
        m_nAspect = 1.3f;
        m_nNear = 0.01f;
        m_nFar = 1000.0f;
        m_nWidth = 1.0;
        m_nHeight = 1.0f;

        m_matWorld = EngineMath::m4_t();
        m_matProj = EngineMath::Projection(m_nFov, m_nAspect,
                                           m_nNear, m_nFar);
        m_nProjectionMode = 0;
    }
    inline virtual ~Camera() {}

    EngineMath::m4_t GetWorldMatrix()
    {
        return m_matWorld;
    }

    EngineMath::m4_t GetViewMatrix()
    {
        if (!IsOrthoNormal())
        {
            OrthoNormalize();
        }
        EngineMath::m4_t inverse = Transpose(m_matWorld);
        inverse._30 = inverse._03 = 0.0f;
        inverse._31 = inverse._13 = 0.0f;
        inverse._32 = inverse._23 = 0.0f;

        EngineMath::Vector3 right = EngineMath::Vector3(m_matWorld._00,
                                                        m_matWorld._01,
                                                        m_matWorld._02);
        EngineMath::Vector3 up = EngineMath::Vector3(m_matWorld._10,
                                                     m_matWorld._11,
                                                     m_matWorld._12);
        EngineMath::Vector3 forward = EngineMath::Vector3(m_matWorld._20,
                                                          m_matWorld._21,
                                                          m_matWorld._22);
        EngineMath::Vector3 position = EngineMath::Vector3(m_matWorld._30,
                                                           m_matWorld._31,
                                                           m_matWorld._32);
        inverse._30 = -right.dot(position);
        inverse._31 = -up.dot(position);
        inverse._32 = -forward.dot(position);

        return inverse;
    }


    EngineMath::m4_t GetProjectionMatrix()
    {
        return m_matProj;
    }

    float GetAspect()
    {
        return m_nAspect;
    }

    bool IsOrthographic()
    {
        return m_nProjectionMode == 1;
    }

    bool IsPerspective()
    {
        return m_nProjectionMode == 0;
    }
    bool IsOrthoNormal()
    {
        EngineMath::Vector3 right = EngineMath::Vector3(m_matWorld._00,
                                                        m_matWorld._01,
                                                        m_matWorld._02);
        EngineMath::Vector3 up = EngineMath::Vector3(m_matWorld._10,
                                                     m_matWorld._11,
                                                     m_matWorld._12);
        EngineMath::Vector3 forward = EngineMath::Vector3(m_matWorld._20,
                                                          m_matWorld._21,
                                                          m_matWorld._22);
        if (!CMP(right.dot(right), 1.0f) ||
            !CMP(up.dot(up), 1.0f) ||
            !CMP(forward.dot(forward), 1.0f))
        {
            return false; // Axis are not normal length
        }
        if (!CMP(forward.dot(up), 0.0f) ||
            !CMP(forward.dot(right), 0.0f) ||
            !CMP(right.dot(up), 0.0f))
        {
            return false; // Axis are not perpendicular
        }
        return true;
    }

    void OrthoNormalize()
    {
        EngineMath::Vector3 right = EngineMath::Vector3(m_matWorld._00,
                                                        m_matWorld._01,
                                                        m_matWorld._02);
        EngineMath::Vector3 up = EngineMath::Vector3(m_matWorld._10,
                                                     m_matWorld._11,
                                                     m_matWorld._12);
        EngineMath::Vector3 forward = EngineMath::Vector3(m_matWorld._20,
                                                          m_matWorld._21,
                                                          m_matWorld._22);
        EngineMath::Vector3 f = forward;
        f.normalize();
        EngineMath::Vector3 r = up.cross(f);
        r.normalize();
        EngineMath::Vector3 u = f.cross(r);
        m_matWorld = EngineMath::m4_t(
            r.x, r.y, r.z, 0.0f,
            u.x, u.y, u.z, 0.0f,
            f.x, f.y, f.z, 0.0f,
            m_matWorld._30,
            m_matWorld._31,
            m_matWorld._32, 1.0f);
    }
    void Resize(int width, int height)
    {
        m_nAspect = (float)width / (float)height;
        if (m_nProjectionMode == 0)
        { // Perspective
            m_matProj = EngineMath::Projection(m_nFov, m_nAspect,
                                               m_nNear, m_nFar);
        }
        else if (m_nProjectionMode == 1)
        { // Ortho
            m_nWidth = (float)width;
            m_nHeight = (float)height;
            float halfW = m_nWidth * 0.5f;
            float halfH = m_nHeight * 0.5f;
            m_matProj = EngineMath::Ortho(-halfW, halfW,
                                          halfH, -halfH, m_nNear, m_nFar);
        }
    }
    void Perspective(float fov, float aspect,
                             float zNear, float zFar)
    {
        m_nFov = fov;
        m_nAspect = aspect;
        m_nNear = zNear;
        m_nFar = zFar;
        m_matProj = EngineMath::Projection(fov, aspect, zNear, zFar);
        m_nProjectionMode = 0;
    }

    void Orthographic(float width, float height,
                              float zNear, float zFar)
    {
        m_nWidth = width;
        m_nHeight = height;
        m_nNear = zNear;
        m_nFar = zFar;
        float halfW = width * 0.5f;
        float halfH = height * 0.5f;
        m_matProj = EngineMath::Ortho(-halfW, halfW,
                                      halfH, -halfH, zNear, zFar);
        m_nProjectionMode = 1;
    }
    void SetProjection(const EngineMath::m4_t &projection)
    {
        m_matProj = projection;
    };
};

class OrbitCamera : public Camera
{
public:
    EngineMath::Vector3 target;
protected:
    EngineMath::Vector2 panSpeed;
    float zoomDistance;
    EngineMath::Vector2 zoomDistanceLimit; // x = min, y = max;
    float zoomSpeed;
    EngineMath::Vector2 rotationSpeed;
    EngineMath::Vector2 yRotationLimit; // x = min, y = max
    EngineMath::Vector2 currentRotation;
    float ClampAngle(float angle, float min, float max)
    {
        while (angle < -360)
        {
            angle += 360;
        }
        while (angle > 360)
        {
            angle -= 360;
        }
        if (angle < min)
        {
            angle = min;
        }
        if (angle > max)
        {
            angle = max;
        }
        return angle;
    }

public:
    OrbitCamera()
    {
        target = EngineMath::Vector3(0, 0, 0);
        zoomDistance = 1.0f;
        zoomSpeed = 200.0f;
        rotationSpeed = EngineMath::Vector2(10.0f, 10.0f);
        yRotationLimit = EngineMath::Vector2(-80.0f, 80.0f);
        zoomDistanceLimit = EngineMath::Vector2(3.0f, 15.0f);
        currentRotation = EngineMath::Vector2(0, 0);
        panSpeed = EngineMath::Vector2(1.0f, 1.0f);
    }
    inline virtual ~OrbitCamera() {}

    void Rotate(const EngineMath::Vector2 &deltaRot,
                             float deltaTime)
    {
        currentRotation.x += deltaRot.x * rotationSpeed.x * zoomDistance * deltaTime;
        currentRotation.y += deltaRot.y * rotationSpeed.y * zoomDistance * deltaTime;
        currentRotation.x = ClampAngle(currentRotation.x,
                                       -360, 360);
        currentRotation.y = ClampAngle(currentRotation.y,
                                       yRotationLimit.x,
                                       yRotationLimit.y);
    }
    void Zoom(float deltaZoom, float deltaTime)
    {
        zoomDistance = zoomDistance + deltaZoom * zoomSpeed * deltaTime;
        if (zoomDistance < zoomDistanceLimit.x)
        {
            zoomDistance = zoomDistanceLimit.x;
        }
        if (zoomDistance > zoomDistanceLimit.y)
        {
            zoomDistance = zoomDistanceLimit.y;
        }
    }
    void Pan(const EngineMath::Vector3 &delataPan,
                          float deltaTime)
    {
        EngineMath::Vector3 right(m_matWorld._00,
                                  m_matWorld._01,
                                  m_matWorld._02);

        EngineMath::Vector3 vertical(m_matWorld._10,
                                  m_matWorld._11,
                                  m_matWorld._12);
        EngineMath::Vector3 forward(m_matWorld._20,
                                  m_matWorld._21,
                                  m_matWorld._22);
        float xPanMag = delataPan.x * panSpeed.x * deltaTime;
        target = target - (right * xPanMag);
        float yPanMag = delataPan.y * panSpeed.y * deltaTime;
        target = target + (vertical * yPanMag);
        float zPanMag = delataPan.z * panSpeed.y * deltaTime;
        target = target - (forward * zPanMag);

    }
    void Update(float dt)
    {
        EngineMath::Vector3 rotation = EngineMath::Vector3(currentRotation.y,
                                                           currentRotation.x,
                                                           0);
        EngineMath::m3_t orient = EngineMath::Rotation3x3(rotation.x,
                                                          rotation.y,
                                                          rotation.z);
        EngineMath::Vector3 direction = MultiplyVector(
            EngineMath::Vector3(0.0, 0.0, -zoomDistance), orient);
        EngineMath::Vector3 position = direction + target;
        m_matWorld = Inverse(
            LookAt(position, target, EngineMath::Vector3(0, 1, 0)));
    }
    void SetTarget(EngineMath::Vector3 atarget) {
        target = atarget;
    }
};

#endif