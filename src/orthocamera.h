#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include "camera.h"

class OrthoCamera : public Camera
{
    float _zoom;
public:
    OrthoCamera(ViewTypes type);

    virtual void UpdateProjection();
    virtual void UpdateView();
    virtual void Move(const glm::vec2 &direction);

    float const &Zoom() const;
    void SetZoom(float zoom);
};

#endif // ORTHOCAMERA_H
