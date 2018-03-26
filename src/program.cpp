#include "program.h"
#include "fpscamera.h"
#include "bsp/bspparser.h"
#include "bsp/bspscene.h"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <imgui_internal.h>
#include <shader.h>

#define SYSTEM_IO_FILEINFO_IMPLEMENTATION
#include <system.io.fileinfo.h>
#include <system.io.path.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <iterator>
#include <string>
#include <thread>
#include <vector>

static std::string mapContent = "{ \
    \"classname\" \"worldspawn\" \
    \"message\" \"DUST by Dave Johnston - for CounterStrike\" \
    \"skyname\" \"des\" \
    \"MaxRange\" \"8000\" \
    \"classname\" \"worldspawn\" \
    \"mapversion\" \"220\" \
    \"wad\" \"\\program files\\valve\\half-life\\valve\\halflife.wad;\\program files\\valve\\half-life\\cstrike\\cs_dust.wad\" \
    { \
        ( 256 -640 -192 ) ( 0 -640 -192 ) ( 0 0 0 ) SANDROAD [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1 \
        ( 256 0 -64 ) ( 0 0 -64 ) ( 0 -640 -256 ) SANDROAD [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1 \
        ( 0 -640 -256 ) ( 0 0 -64 ) ( 0 0 0 ) SANDROAD [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1 \
        ( 256 -640 -192 ) ( 256 0 0 ) ( 256 0 -64 ) SANDROAD [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1 \
        ( 0 0 -64 ) ( 256 0 -64 ) ( 256 0 0 ) SANDROAD [ 1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1 \
        ( 0 -640 -192 ) ( 256 -640 -192 ) ( 256 -640 -256 ) SANDROAD [ 1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1 \
    } \
}";

Program::Program(GLFWwindow *window)
    : _window(window), _running(true), _statusProgress(-1.0f)
{
    std::lock_guard<std::mutex> lock(_stateMutex);
    app.search_for[0] = '\0';

    view._camera = new FpsCamera(ViewTypes::PerspectiveFlat);

    glfwSetWindowUserPointer(this->_window, static_cast<void *>(this));
}

Program::~Program()
{
    glfwSetWindowUserPointer(this->_window, nullptr);
}

void Program::SetInputFile(System::IO::FileInfo const &file)
{
    if (file.Exists() && file.Name() == "hl.exe")
    {
        populateMapsManagerFromOptions(file);
    }
}

void Program::populateMapsManagerFromOptions(System::IO::FileInfo const &hl)
{
    std::thread t([this, hl]() mutable {
        auto hlPath = hl.Directory();

        std::vector<System::IO::FileInfo> filesToLoad;
        auto dirs = hlPath.GetDirectories();
        for (auto sub : dirs)
        {
            System::IO::DirectoryInfo path(System::IO::Path::Combine(hlPath.FullName(), sub));
            auto files = path.GetFiles();
            for (auto file : files)
            {
                auto fileInfo = System::IO::FileInfo(System::IO::Path::Combine(path.FullName(), file));
                filesToLoad.push_back(fileInfo);
            }
        }

        int c = 0;
        for (auto fileInfo : filesToLoad)
        {

            c++;
        }

        setStatus(-1.0f, "");
    });

    t.detach();
}

void Program::setStatus(float state, std::string const &message)
{
    std::lock_guard<std::mutex> lock(_statusbarMutex);

    _statusProgress = state;
    _statusMessage = message;
}

bool Program::SetUp()
{
    modal.show = false;
    modal.message = "";

    ImGuiIO &io = ImGui::GetIO();
    if (io.Fonts->AddFontFromFileTTF("c:\\windows\\fonts\\verdana.ttf", 16.0f) == nullptr)
    {
        return false;
    }

    app.shader = LoadShaderProgram("mapShader");

    glUseProgram(app.shader);

    app.u_tex = glGetUniformLocation(app.shader, "u_tex");
    app.u_p = glGetUniformLocation(app.shader, "u_p");
    app.u_v = glGetUniformLocation(app.shader, "u_v");
    app.u_m = glGetUniformLocation(app.shader, "u_m");

    glUseProgram(0);

    glGenVertexArrays(1, &app.vao);
    glGenBuffers(1, &app.vbo);

    std::ifstream file("C:\\Steam\\steamapps\\common\\Half-Life\\cstrike\\maps\\cs_office.bsp", std::ios::in | std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        if (file.tellg() > 0)
        {
            int size = file.tellg();
            auto data = new unsigned char[size];
            file.seekg(0, std::ios::beg);
            file.read((char*)data, size);

            BspParser parser(data, size);
            BspScene scene;

            parser.LoadScene(&scene);
        }
        file.close();
    }

    return true;
}

void Program::onResize(int width, int height)
{
    app.width = width;
    app.height = height;

    view._camera->SetViewRect(glm::vec4(0.0f, 40.0f, app.width, app.height - 40.0f - 20.0f));
}

void Program::Render()
{
    glViewport(0, menubarHeight, app.width, app.height - menubarHeight - statusbarHeight);
    glClearColor(114 / 255.0f, 144 / 255.0f, 154 / 255.0f, 255 / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui_ImplGlfwGL3_NewFrame();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    {
        renderGuiMenu();

        renderView(view._camera->ViewRect());

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.20f, 0.47f, 0.60f));
        {
            ImGui::Begin("statusbar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            {
                std::lock_guard<std::mutex> lock(_statusbarMutex);
                ImGui::SetWindowPos(ImVec2(0, app.height - statusbarHeight));
                ImGui::SetWindowSize(ImVec2(app.width, statusbarHeight));

                ImGui::Columns(2);
                ImGui::Text(_statusMessage.c_str());
                ImGui::NextColumn();
                if (_statusProgress >= 0.0f)
                {
                    ImGui::ProgressBar(_statusProgress);
                }
                ImGui::NextColumn();
            }
            ImGui::End();
        }
        ImGui::PopStyleColor();
    }
    ImGui::PopStyleVar();

    ImGui::Render();

    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::onKeyAction(int key, int scancode, int action, int mods)
{
    app.shiftPressed = (mods & GLFW_MOD_SHIFT);
    app.ctrlPressed = (mods & GLFW_MOD_CONTROL);
}

void Program::onMouseMove(int x, int y)
{
    app.mousex = x;
    app.mousey = y;
}

void Program::onMouseButton(int button, int action, int mods)
{
}

void Program::onScroll(int x, int y)
{
    if (app.shiftPressed)
    {
    }
    else if (app.ctrlPressed)
    {
    }
    else
    {
    }
}

void Program::CleanUp() {}

void Program::KeyActionCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
        app->onKeyAction(key, scancode, action, mods);
}

void Program::CursorPosCallback(GLFWwindow *window, double x, double y)
{
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
        app->onMouseMove(int(x), int(y));
}

void Program::ScrollCallback(GLFWwindow *window, double x, double y)
{
    ImGui_ImplGlfw_ScrollCallback(window, x, y);

    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
        app->onScroll(int(x), int(y));
}

void Program::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
        app->onMouseButton(button, action, mods);
}

void Program::ResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
        app->onResize(width, height);
}
