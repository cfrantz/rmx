#include "gfx/camera.h"

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace GFX {

void Camera::Init(GLuint program) {
    loc_.up      = glGetUniformLocation(program, "g_camUp");
    loc_.right   = glGetUniformLocation(program, "g_camRight");
    loc_.forward = glGetUniformLocation(program, "g_camForward");
    loc_.eye     = glGetUniformLocation(program, "g_eye");
    loc_.focal_length = glGetUniformLocation(program, "g_focalLength");
    loc_.near    = glGetUniformLocation(program, "g_zNear");
    loc_.far     = glGetUniformLocation(program, "g_ZFar");
}

void Camera::Update() {
    glUniform3fv(loc_.up, 1, glm::value_ptr(up_));
    glUniform3fv(loc_.right, 1, glm::value_ptr(right_));
    glUniform3fv(loc_.forward, 1, glm::value_ptr(forward_));
    glUniform3fv(loc_.eye, 1, glm::value_ptr(eye_));
    glUniform1f(loc_.focal_length, focal_length_);

    // maybe don't have to do these on every frame.
    glUniform1f(loc_.near, near_);
    glUniform1f(loc_.far, far_);
}

}  // namespace GFX
