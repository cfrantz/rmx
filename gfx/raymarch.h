#ifndef RMX_GFX_RAYMARCH_H
#define RMX_GFX_RAYMARCH_H
#include <memory>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "gfx/camera.h"
#include "gfx/shader.h"

namespace GFX {

class RayMarchScene {
  public:
    RayMarchScene(int width, int height)
      : sky_color_(glm::vec4(0.31f, 0.47f, 0.67f, 1.0f)),
        ambient_(glm::vec4(0.15f, 0.20f, 0.32f, 1.0f)),
        light0pos_(glm::vec3(0.25f, 4.0f, 0.0f)),
        light0col_(glm::vec4(0.67f, 0.87f, 0.93f, 1.0f)),
        op_(0),
        width_(width),
        height_(height),
        aspect_ratio_(float(width)/float(height)),
        steps_(64),
        epsilon_(0.001f)
    {}

    bool LoadProgram(const std::string& vs, const std::string& fs);
    void Init();
    void Draw();

    inline Camera* camera() { return &camera_; }

    glm::vec4 sky_color_;
    glm::vec4 ambient_;
    glm::vec3 light0pos_;
    glm::vec4 light0col_;
    int op_;

  private:
    int width_;
    int height_;
    float aspect_ratio_;
    int steps_;
    float epsilon_;

    GLuint vao_;
    GLuint vbo_;

    Camera camera_;
    std::unique_ptr<Shader> shader_;


    struct Locations {
        GLuint sky_color;
        GLuint ambient;
        GLuint light0pos;
        GLuint light0col;
        GLuint op;

        GLuint resolution;
        GLuint aspect_ratio;
        GLuint steps;
        GLuint epsilon;

        // Vertex shader
        GLuint position;
    } loc_;
};

}  // namespace GFX

#endif // RMX_GFX_RAYMARCH_H
