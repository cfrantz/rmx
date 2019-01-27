#include "gfx/swmarch.h"

#include "glm/glm.hpp"
#include "imwidget/glbitmap.h"

namespace GFX {
using namespace glm;

void SWMarcher::Draw() {
    Render();
    bitmap_.DrawAt(0, 0, 2.0f);
}

// The Render function simulates what the GPU would do: execute the
// shader program for every point in the image.
void SWMarcher::Render() {
    float ustep = 2.0f / bitmap_.width();
    float vstep = 2.0f / bitmap_.height();
    float u, v;

    v = 1.0f;
    for(int y=0; y<bitmap_.height(); ++y, v-=vstep) {
        u = -1.0f;
        for(int x=0; x<bitmap_.width(); ++x, u+=ustep) {
            vec4 color = RenderMain(vec2(u, v));
            color = clamp(color, 0.0f, 1.0f) * 255.0f;
            // We want ABGR ordering.
            uint32_t pixel = uint32_t(color.r) <<  0 |
                             uint32_t(color.g) <<  8 |
                             uint32_t(color.b) << 16 |
                             uint32_t(color.a) << 24 ;
            bitmap_.SetPixel(x, y, pixel);
        }
    }
    bitmap_.Update();
}

//========================================================================
//
// The following code is very similar to a fragment shader program for
// a GPU.
//
//========================================================================

// Maps x from the range [minX, maxX] to the range [minY, maxY]
// The function does not clamp the result, as it may be useful
float mapTo(float x, float minX, float maxX, float minY, float maxY)
{
    float a = (maxY - minY) / (maxX - minX);
    float b = minY - a * minX;
    return a * x + b;
}

vec4 SWMarcher::GetFloorTexture(const vec3& pos) {
    // Compute a checkerboard texture
    vec2 m = pos.xz;
    m = mod(m, 2.0f) - 1.0f;
    return m.x * m.y > 0.0f ? vec4(0.1f) : vec4(1.0f);
}

float SWMarcher::RaytraceFloor(
        const vec3& ro, const vec3& rd,
        const vec3& normal, const vec3& pos) {
    return dot(pos - ro, normal) / dot(rd, normal);
}

vec4 SWMarcher::ComputeColor(const vec3& ro, const vec3& rd) {
    vec3 floor_normal = vec3(0, 1, 0);
    vec3 floor_pos = vec3(0, -0.5f, 0);

    float t;        // Distance travelled by ray to eye
    vec3 p;         // Surface point
    vec3 normal;    // Surface normal
    vec4 texture;   // Surface texture

    float t0 = camera_.far;
    float t1 = RaytraceFloor(ro, rd, floor_normal, floor_pos);

    // Check if floor was closet and in view of the camera.
    if (t1 < t0 && t1 >= camera_.near && t1 < camera_.far) {
        t = t1;
        p = ro + rd*t;
        normal = floor_normal;
        texture = GetFloorTexture(p);
    } else {
        return sky_color_;
    }

    vec4 color;
    float z = mapTo(t, camera_.near, camera_.far, 1, 0);
    color = vec4(1.0f) * z * texture;
    return color;
}

// The RenderMain function is similar to the fragment shader main() function.
vec4 SWMarcher::RenderMain(const vec2& uv) {
    vec3 rayorigin = camera_.eye;
    vec3 raydirection = normalize(camera_.forward * camera_.focal_length +
                                  camera_.right * uv.x * aspect_ratio_ +
                                  camera_.up * uv.y);

    // If you want to validate that uv sweeps over (-1,-1) to (1, 1)
    //vec4 color = vec4(0, uv.x*0.5f+0.5f, uv.y*0.5f+0.5f, 1.0f);

    vec4 color = ComputeColor(rayorigin, raydirection);
    return color;
}

}  // namespace GFX
