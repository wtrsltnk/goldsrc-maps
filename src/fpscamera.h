#ifndef FPSCAMERA_H
#define FPSCAMERA_H

#include <glm/glm.hpp>
#include "camera.h"

class FpsCamera : public Camera
{
public:
    FpsCamera(ViewTypes type);
    virtual ~FpsCamera();

    virtual void UpdateProjection();
    virtual void UpdateView();
    virtual void Move(glm::vec2 const &direction);

    float const &Pitch() const;
    void SetPitch(float pitch);

    float const &Roll() const;
    void SetRoll(float roll);

    float const &Yaw() const;
    void SetYaw(float yaw);

private:
    float _pitch;
    float _roll;
    float _yaw;

};

#endif // FPSCAMERA_H
