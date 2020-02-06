#include "program.h"
#include <glm/gtc/type_ptr.hpp>

void Program::renderView(
    glm::vec4 const &viewRect)
{
    glViewport(viewRect.x, viewRect.y, viewRect.z, viewRect.w);

    glScissor(viewRect.x, viewRect.y, viewRect.z, viewRect.w);
    glEnable(GL_SCISSOR_TEST);

    glClearColor(14 / 255.0f, 44 / 255.0f, (app.focusedView ? 60 : 54) / 255.0f, 155 / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT_AND_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(app.shader);

    GLenum mode = GL_TRIANGLE_FAN;
    auto projectionMatrix = view._camera->ProjectionMatrix();
    auto viewMatrix = view._camera->ViewMatrix();
    auto modelMatrix = glm::mat4(1.0f);

    glUniform1i(app.u_tex, 0);
    glUniformMatrix4fv(app.u_p, 1, false, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(app.u_v, 1, false, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(app.u_m, 1, false, glm::value_ptr(modelMatrix));

    glBindVertexArray(app.vao);
    glBindBuffer(GL_ARRAY_BUFFER, app.vbo);

    for (auto face : doc.renderData._data)
    {
        glDrawArrays(mode, face.firstVertex, face.vertexCount);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_SCISSOR_TEST);
}
