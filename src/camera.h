#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

enum class ViewTypes
{
    Top,
    Front,
    Side,
    PerspectiveWireframe,
    PerspectiveFlat,
    PerspectiveTextured
};

class Camera
{
protected:
    glm::vec4 _viewRect;
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::vec3 _eye;
    ViewTypes _viewType;
    bool _mouseLookMode;

    Camera(ViewTypes type);
public:
    virtual ~Camera();

    virtual void UpdateProjection() = 0;
    virtual void UpdateView() = 0;
    virtual void Move(glm::vec2 const &direction) = 0;

    glm::vec4 const &ViewRect() const;
    void SetViewRect(glm::vec4 const &rect);

    glm::mat4 const &ProjectionMatrix() const;
    glm::mat4 const &ViewMatrix() const;

    glm::vec3 const &Eye() const;
    void SetEye(glm::vec3 const &eye);

    ViewTypes const &ViewType() const;
    void SetViewType(ViewTypes type);

    bool MouseLookMode() const;
    void SetMouseLookMode(bool enabled);

    bool IsPerspectiveViewType() const;
};

#endif // CAMERA_H
