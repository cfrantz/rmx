#include "gfx/raymarch.h"

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace GFX {
bool RayMarchScene::LoadProgram(const std::string& vs, const std::string& fs) {
    auto p = Shader::Load(vs, fs, "");
    if (!p) {
        return false;
    }
    shader_ = std::move(p);
    return true; 
}

void RayMarchScene::Init() {
    GLuint program = shader_->program();

    shader_->Use();
    camera_.Init(program);
    loc_.resolution =   glGetUniformLocation(program, "g_resolution");
    loc_.aspect_ratio = glGetUniformLocation(program, "g_aspectRatio");
    loc_.steps =        glGetUniformLocation(program, "g_rmSteps");
    loc_.epsilon =      glGetUniformLocation(program, "g_rmEpsilon");
    loc_.sky_color =    glGetUniformLocation(program, "g_skyColor");
    loc_.ambient =      glGetUniformLocation(program, "g_ambient");
    loc_.light0pos =    glGetUniformLocation(program, "g_light0Position");
    loc_.light0col =    glGetUniformLocation(program, "g_light0Color");
    loc_.position =     glGetAttribLocation(program, "position");


    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
}

void RayMarchScene::Draw() {
    camera_.Update();
    glUniform4fv(loc_.sky_color, 1, glm::value_ptr(sky_color_));
    glUniform4fv(loc_.ambient, 1, glm::value_ptr(ambient_));
    glUniform4fv(loc_.light0pos, 1, glm::value_ptr(light0pos_));
    glUniform4fv(loc_.light0col, 1, glm::value_ptr(light0col_));

    GLfloat vertices[] = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
    };
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnableVertexAttribArray(loc_.position);
    glVertexAttribPointer(loc_.position, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

}  // namespace GFX
