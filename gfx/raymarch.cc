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
    loc_.resolution =   glGetUniformLocation(program, "scene_resolution");
    loc_.aspect_ratio = glGetUniformLocation(program, "scene_aspect_ratio");
    loc_.steps =        glGetUniformLocation(program, "scene_steps");
    loc_.epsilon =      glGetUniformLocation(program, "scene_epsilon");
    loc_.sky_color =    glGetUniformLocation(program, "scene_sky_color");
    loc_.ambient =      glGetUniformLocation(program, "scene_ambient");
    loc_.light0pos =    glGetUniformLocation(program, "scene_light0pos");
    loc_.light0col =    glGetUniformLocation(program, "scene_light0col");
    loc_.position =     glGetAttribLocation(program, "position");


    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
}

void RayMarchScene::Draw() {
    camera_.Update();
    glUniform2f(loc_.resolution, width_, height_);
    glUniform1f(loc_.aspect_ratio, aspect_ratio_);
    glUniform1f(loc_.epsilon, epsilon_);
    glUniform1i(loc_.steps, steps_);
    glUniform4fv(loc_.sky_color, 1, glm::value_ptr(sky_color_));
    glUniform4fv(loc_.ambient, 1, glm::value_ptr(ambient_));
    glUniform3fv(loc_.light0pos, 1, glm::value_ptr(light0pos_));
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
    glEnableVertexAttribArray(loc_.position);
    glVertexAttribPointer(loc_.position, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}  // namespace GFX
