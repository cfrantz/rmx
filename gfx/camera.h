#ifndef RMX_GFX_CAMERA_H
#define RMX_GFX_CAMERA_H
#include <GL/glew.h>

#include "glm/glm.hpp"

namespace GFX {
class Camera {
  public:
    Camera()
      : up(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))),
        right(glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))),
        forward(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))),
        eye(glm::vec3(0.0f, 0.0f, -2.0f)),
        focal_length(1.67f),
        near(0.0f),
        far(150.0f) {}

    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;
    glm::vec3 eye;
    float focal_length;
    float near;
    float far;

    void Init(GLuint program);
    void Update();

  private:
    struct Locations {
        GLuint up;
        GLuint right;
        GLuint forward;
        GLuint eye;
        GLuint focal_length;
        GLuint near;
        GLuint far;
    } loc_;
};

}  // namespace GFX
#endif // RMX_GFX_CAMERA_H
