#ifndef RMX_GFX_SWMARCH_H
#define RMX_GFX_SWMARCH_H

#include "glm/glm.hpp"
#include "imwidget/glbitmap.h"

namespace GFX {

class SWMarcher {
  public:
    struct Camera {
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 forward;
        glm::vec3 eye;
        float focal_length;
        float near;
        float far;
    };

    SWMarcher(int width, int height)
      : bitmap_(width, height),
        aspect_ratio_(float(width)/float(height)),
        steps_(64),
        epsilon_(0.001f),
        sky_color_(glm::vec4(0.31f, 0.47f, 0.67f, 1.0f)),
        ambient_(glm::vec4(0.15, 0.20, 0.32, 1.0f)),
        camera_{
            glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
            glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)),
            glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)),
            glm::vec3(0.0f, 0.0f, -2.0f),
            1.67f,
            0.0f,
            150.0f
        } {}
    
    void Render();
    void Draw();
    inline Camera* camera() { return &camera_; }

    // These methods implement the ray marcher, and should be very similar
    // to what you'd implement in a fragment shader.
    // Common abbrieviations:
    //   ro -> rayorigin
    //   rd -> raydirection
    //
    glm::vec4 RenderMain(const glm::vec2& uv);
    glm::vec4 ComputeColor(const glm::vec3& rayorigin, const glm::vec3& raydirection);
    glm::vec4 GetFloorTexture(const glm::vec3& pos);
    float RaytraceFloor(
            const glm::vec3& ro, const glm::vec3& rd,
            const glm::vec3& normal, const glm::vec3& pos);
    void RayMarch(
            const glm::vec3& ro, const glm::vec3& rd,
            int& steps, float& distance);


  private:
    GLBitmap bitmap_;
  public:
    float aspect_ratio_;
    int steps_;
    float epsilon_;
    glm::vec4 sky_color_;
    glm::vec4 ambient_;
  private:
    Camera camera_;
};

}  // namespace GFX
#endif // RMX_GFX_SWMARCH_H
