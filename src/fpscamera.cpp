#include "fpscamera.h"
#include <glm/gtc/matrix_transform.hpp>

FpsCamera::FpsCamera(ViewTypes type)
    : Camera(type), _pitch(glm::radians(-180.0f)), _roll(glm::radians(-180.0f)), _yaw(glm::radians(0.0f))
{
    UpdateView();
    UpdateProjection();
}

FpsCamera::~FpsCamera()
{}

void FpsCamera::UpdateProjection()
{
    auto aspect = 1.0f;
    if (_viewRect.w > 0.0f)
    {
        aspect = _viewRect.z / _viewRect.w;
    }
    _projectionMatrix = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 4096.0f);
}

void FpsCamera::UpdateView()
{
    glm::mat4 matRoll = glm::mat4(1.0f);
    glm::mat4 matPitch = glm::mat4(1.0f);
    glm::mat4 matYaw = glm::mat4(1.0f);

    matRoll = glm::rotate(matRoll, Roll(), glm::vec3(0.0f, 0.0f, 1.0f));
    matPitch = glm::rotate(matPitch, Pitch(), glm::vec3(1.0f, 0.0f, 0.0f));
    matYaw = glm::rotate(matYaw, Yaw(), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 rotate = matRoll * matPitch * matYaw;

    auto qm = glm::mat4(glm::vec4(0, 0, -1, 0),
                            glm::vec4(-1, 0, 0, 0),
                            glm::vec4(0, 1, 0, 0),
                            glm::vec4(0, 0, 0, 1));

    _viewMatrix = rotate * glm::translate(glm::mat4(1.0f), -Eye()) * qm;
}

float const &FpsCamera::Pitch() const
{
    return _pitch;
}

void FpsCamera::SetPitch(float pitch)
{
    _pitch = pitch;

    if (_pitch < glm::radians(-270.0f))
    {
        _pitch = glm::radians(-270.0f);
    }

    if (_pitch > glm::radians(-90.0f))
    {
        _pitch = glm::radians(-90.0f);
    }
}

float const &FpsCamera::Roll() const
{
    return _roll;
}

void FpsCamera::SetRoll(float roll)
{
    _roll = roll;
}

float const &FpsCamera::Yaw() const
{
    return _yaw;
}

void FpsCamera::SetYaw(float yaw)
{
    _yaw = yaw;
}

void FpsCamera::Move(glm::vec2 const &direction)
{
    glm::mat4 matRoll = glm::mat4(1.0f);
    glm::mat4 matPitch = glm::mat4(1.0f);
    glm::mat4 matYaw = glm::mat4(1.0f);

    matRoll = glm::rotate(matRoll, Roll(), glm::vec3(0.0f, 0.0f, 1.0f));
    matPitch = glm::rotate(matPitch, Pitch(), glm::vec3(1.0f, 0.0f, 0.0f));
    matYaw = glm::rotate(matYaw, Yaw(), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 rotate = glm::transpose(matRoll * matPitch * matYaw);

    auto forward = glm::vec3(
            (rotate[0].x * -direction.x),
            (rotate[0].y * -direction.x),
            (rotate[0].z * -direction.x)
        );
    auto left = glm::vec3(
            (rotate[2].x * -direction.y),
            (rotate[2].y * -direction.y),
            (rotate[2].z * -direction.y)
        );
    _eye += forward + left;

    UpdateView();
}
