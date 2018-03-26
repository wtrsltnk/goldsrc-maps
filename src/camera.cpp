#include "camera.h"

Camera::Camera(ViewTypes type)
    : _viewType(type), _mouseLookMode(false),
      _eye(glm::vec3(0.0f)),
      _viewRect(glm::vec4(0.0f))
{
}

Camera::~Camera()
{}

glm::vec4 const &Camera::ViewRect() const
{
    return _viewRect;
}

void Camera::SetViewRect(glm::vec4 const &rect)
{
    _viewRect = rect;

    UpdateProjection();
}

glm::mat4 const &Camera::ProjectionMatrix() const
{
    return _projectionMatrix;
}

glm::mat4 const &Camera::ViewMatrix() const
{
    return _viewMatrix;
}

glm::vec3 const &Camera::Eye() const
{
    return _eye;
}

void Camera::SetEye(glm::vec3 const &eye)
{
    _eye = eye;
}

ViewTypes const &Camera::ViewType() const
{
    return _viewType;
}

void Camera::SetViewType(ViewTypes type)
{
    _viewType = type;
}

bool Camera::MouseLookMode() const
{
    return _mouseLookMode;
}

void Camera::SetMouseLookMode(bool enabled)
{
    _mouseLookMode = enabled;
}

bool Camera::IsPerspectiveViewType() const
{
    if (_viewType == ViewTypes::PerspectiveFlat)
    {
        return true;
    }
    if (_viewType == ViewTypes::PerspectiveTextured)
    {
        return true;
    }
    if (_viewType == ViewTypes::PerspectiveWireframe)
    {
        return true;
    }

    return false;
}
