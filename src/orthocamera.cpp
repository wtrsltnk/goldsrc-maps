#include "orthocamera.h"
#include <glm/gtc/matrix_transform.hpp>

OrthoCamera::OrthoCamera(ViewTypes type)
    : Camera(type), _zoom(1.0f)
{
    UpdateView();
    UpdateProjection();
}

void OrthoCamera::UpdateProjection()
{
    float halfw = float(_viewRect.z / 2);
    float halfh = float(_viewRect.w / 2);
    _projectionMatrix = glm::ortho(-halfw, halfw, -halfh, halfh, -4096.0f, 4096.0f);
}

void OrthoCamera::UpdateView()
{
    auto view = glm::scale(glm::mat4(1.0f), glm::vec3(_zoom));
    view = glm::translate(view, glm::vec3(Eye().x, Eye().y, 0.0f));
    if (ViewType() == ViewTypes::Top)
    {
        _viewMatrix = view;
    }
    else if (ViewType() == ViewTypes::Side)
    {
        _viewMatrix = glm::rotate(view, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    else if (ViewType() == ViewTypes::Front)
    {
        view = glm::rotate(view, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        _viewMatrix = glm::rotate(view, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
}

void OrthoCamera::Move(const glm::vec2 &direction)
{
    _eye.x -= direction.x;
    _eye.y += direction.y;

    UpdateView();
}

float const &OrthoCamera::Zoom() const
{
    return _zoom;
}

void OrthoCamera::SetZoom(float zoom)
{
    _zoom = zoom;
}
