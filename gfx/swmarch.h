#ifndef RMX_GFX_SWMARCH_H
#define RMX_GFX_SWMARCH_H

#include "gfx/camera.h"
#include "glm/glm.hpp"
#include "imwidget/glbitmap.h"

namespace GFX {

class SWMarcher {
  public:
    SWMarcher(int width, int height)
      : bitmap_(width, height),
        aspect_ratio_(float(width)/float(height)),
        steps_(64),
        epsilon_(0.001f),
        sky_color_(glm::vec4(0.31f, 0.47f, 0.67f, 1.0f)),
        ambient_(glm::vec4(0.15, 0.20, 0.32, 1.0f)),
        light0pos_(glm::vec3(0.0f, 3.0f, 0.0f)),
        light0col_(glm::vec4(1))
        {}
    
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
    float GetVisibility(const glm::vec3& p0, const glm::vec3& p1, float k);
    glm::vec4 GetShading(
            const glm::vec3& pos, const glm::vec3& normal,
            const glm::vec3& light_pos, const glm::vec4& light_col);
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
    glm::vec3 light0pos_;
    glm::vec4 light0col_;
  private:
    Camera camera_;
};

}  // namespace GFX
#endif // RMX_GFX_SWMARCH_H
