#ifndef RMX_GFX_CAMERA_H
#define RMX_GFX_CAMERA_H
#include <GL/glew.h>

#include "glm/glm.hpp"

namespace GFX {
class Camera {
  public:
    Camera()
      : up_(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))),
        right_(glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))),
        forward_(glm::cross(right_, up_)),
        eye_(glm::vec3(0.0f, 0.0f, -2.0f)),
        focal_length_(1.67f),
        near_(0.0f),
        far_(150.0f) {}

    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 forward_;
    glm::vec3 eye_;
    float focal_length_;
    float near_;
    float far_;

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
