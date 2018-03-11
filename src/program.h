#ifndef PROGRAM_H
#define PROGRAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <system.io.fileinfo.h>
#include <mutex>

#include "map/mapscene.h"
#include "map/maprenderdata.h"

struct app_state
{
    bool show_texture_browser = true;
    bool show_texture_view = true;
    float width = 200.0f;
    float height = 300.0f;
    int mousex = 0;
    int mousey = 0;
    bool shiftPressed = false;
    bool ctrlPressed = false;
    char search_for[45];
    bool focusedView = false;
    GLuint vao, vbo, shader;
    GLuint u_tex, u_p, u_v, u_m;
};

struct doc_state
{
    MapScene scene;
    MapRenderData renderData;
};

struct view_state
{
    glm::vec3 eye = glm::vec3(0.0f);

    // For 3D views
    float pitch = 0.0f;
    float yaw = 0.0f;

    // For 2d views
    float zoom = 1.0f;
};

struct message_modal
{
    bool show;
    std::string message;
};

class Program
{
private:
    GLFWwindow *_window;

    const int menubarHeight = 22;
    const int statusbarHeight = 40;

    std::mutex _stateMutex;
    void updateTextureBrowser();

    app_state app;
    doc_state doc;
    view_state view;
    message_modal modal;

    std::mutex _statusbarMutex;
    std::string _statusMessage;
    float _statusProgress;
    void setStatus(float app, std::string const &message);

    void populateMapsManagerFromOptions(System::IO::FileInfo const &hl);

    void renderGuiMenu();
    void renderGuiAbout();
    void renderView(glm::vec4 const & rect);

    bool openAsset();
    void quitApp();

public:
    static void KeyActionCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void ResizeCallback(GLFWwindow *window, int width, int height);
    static void CursorPosCallback(GLFWwindow *window, double x, double y);
    static void ScrollCallback(GLFWwindow *window, double x, double y);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

protected:
    void onKeyAction(int key, int scancode, int action, int mods);
    void onResize(int width, int height);
    void onMouseMove(int x, int y);
    void onMouseButton(int button, int action, int mods);
    void onScroll(int x, int y);

public:
    Program(GLFWwindow *window);
    virtual ~Program();

    void SetInputFile(System::IO::FileInfo const &file);

    bool SetUp();
    void Render();
    void CleanUp();

    bool _running;
};

#endif // PROGRAM_H
