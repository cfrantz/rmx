#include "gfx/camera.h"

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace GFX {

void Camera::Init(GLuint program) {
    loc_.up      =      glGetUniformLocation(program, "camera_up");
    loc_.right   =      glGetUniformLocation(program, "camera_right");
    loc_.forward =      glGetUniformLocation(program, "camera_forward");
    loc_.eye     =      glGetUniformLocation(program, "camera_eye");
    loc_.focal_length = glGetUniformLocation(program, "camera_focal_length");
    loc_.near    =      glGetUniformLocation(program, "camera_near");
    loc_.far     =      glGetUniformLocation(program, "camera_far");
}

void Camera::Update() {
    glUniform3fv(loc_.up, 1, glm::value_ptr(up));
    glUniform3fv(loc_.right, 1, glm::value_ptr(right));
    glUniform3fv(loc_.forward, 1, glm::value_ptr(forward));
    glUniform3fv(loc_.eye, 1, glm::value_ptr(eye));
    glUniform1f(loc_.focal_length, focal_length);

    // maybe don't have to do these on every frame.
    glUniform1f(loc_.near, near);
    glUniform1f(loc_.far, far);
}

}  // namespace GFX
