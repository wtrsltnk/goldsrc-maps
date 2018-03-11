#include "program.h"
#include <glm/gtc/type_ptr.hpp>

glm::mat4 FPSViewRH(glm::vec3 const &eye, float pitch, float yaw)
{
    // If the pitch and yaw angles are in degrees,
    // they need to be converted to radians. Here
    // I assume the values are already converted to radians.
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);

    glm::vec3 xaxis = {cosYaw, 0, -sinYaw};
    glm::vec3 yaxis = {sinYaw * sinPitch, cosPitch, cosYaw * sinPitch};
    glm::vec3 zaxis = {sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw};

    // Create a 4x4 view matrix from the right, up, forward and eye position
    // vectors
    glm::mat4 viewMatrix = {glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0),
                            glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0),
                            glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0),
                            glm::vec4(-glm::dot(xaxis, eye),
                                      -glm::dot(yaxis, eye),
                                      -glm::dot(zaxis, eye), 1)};

    return viewMatrix;
}

void Program::renderView(glm::vec4 const & viewRect)
{
    glViewport(viewRect.x, viewRect.y, viewRect.z, viewRect.w);

    glScissor(viewRect.x, viewRect.y, viewRect.z, viewRect.w);
    glEnable(GL_SCISSOR_TEST);

    glClearColor(14 / 255.0f, 44 / 255.0f, (app.focusedView ? 60 : 54) / 255.0f, 155 / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    doc.renderData.Collect(&(doc.scene));
    if (doc.renderData._verts.size() > 0)
    {
        glDisable(GL_DEPTH_TEST);
        glCullFace(GL_FRONT_AND_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glUseProgram(app.shader);

        GLenum mode = GL_TRIANGLE_FAN;
        auto projectionMatrix = glm::perspective(glm::radians(60.0f), viewRect.z / viewRect.w, 0.1f, 4096.0f);
        auto viewMatrix = FPSViewRH(view.eye, view.pitch, view.yaw);
        auto modelMatrix = glm::mat4(1.0f);

        glUniform1i(app.u_tex, 0);
        glUniformMatrix4fv(app.u_p, 1, false, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(app.u_v, 1, false, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(app.u_m, 1, false, glm::value_ptr(modelMatrix));

        glBindVertexArray(app.vao);
        glBindBuffer(GL_ARRAY_BUFFER, app.vbo);

        for (auto t : doc.renderData._data)
        {
            for (const MapFace &face : t.second)
            {
                glDrawArrays(mode, face._firstVertex, face._vertexCount);
            }
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glUseProgram(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glDisable(GL_SCISSOR_TEST);
}
