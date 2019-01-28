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

float vmax(const glm::vec3& v) {
    return max(max(v.x, v.y), v.z);
}

float fSphere(const glm::vec3& position, float radius) {
    return length(position) - radius;
}

float fBoxCheap(const glm::vec3& position, const glm::vec3& size) {
    return vmax(abs(position) - size);
}

float DistScene(const glm::vec3& position) {
    return fSphere(position, 0.4f);
    //return fBoxCheap(position, vec3(1.0f, 0.25f, 0.333f));
}

// Approximate the normalized gradient of the distance function at point p.
// If p is near a surface, the gradient will approximate the surface normal.
vec3 GetNormal(const glm::vec3& p) {
    float h = 0.0001f;
    return normalize(vec3(
        DistScene(p + vec3(h, 0, 0)) - DistScene(p - vec3(h, 0, 0)),
        DistScene(p + vec3(0, h, 0)) - DistScene(p - vec3(0, h, 0)),
        DistScene(p + vec3(0, 0, h)) - DistScene(p - vec3(0, 0, h))));
}

void SWMarcher::RayMarch(
        const glm::vec3& ro, const glm::vec3& rd,
        int& i, float& distance) {
    distance = 0.0f;
    for(i=0; i<steps_; ++i) {
        float d = DistScene(ro + rd * distance);

        // Make epsilon proportional to the distance so that accuracy can
        // drop as we get further into the scene.  We also just drop the
        // ray if it goes outside the far camera bound.
        if (d < epsilon_*distance*2.0f || distance >= camera_.far) {
            break;
        }
        distance += d;
    }
}

vec4 SWMarcher::GetFloorTexture(const vec3& pos) {
    // Compute a checkerboard texture
    vec2 m = pos.xz;
    m = mod(m, 2.0f) - 1.0f;
    return m.x * m.y > 0.0f ? vec4(0.333f) : vec4(1.0f);
}

float SWMarcher::RaytraceFloor(
        const vec3& ro, const vec3& rd,
        const vec3& normal, const vec3& pos) {
    return dot(pos - ro, normal) / dot(rd, normal);
}

vec4 SWMarcher::ComputeColor(const vec3& ro, const vec3& rd) {
    vec3 floor_normal = vec3(0, 1, 0);
    vec3 floor_pos = vec3(0, -0.5f, 0);

    float t;                    // Distance travelled by ray to eye
    vec3 p;                     // Surface point
    vec3 normal;                // Surface normal
    vec4 texture = vec4(1.0f);  // Surface texture

    int i;          // Steps traveled in raymarch
    float t0;       // Distance traveled in raymarch
    RayMarch(ro, rd, i, t0);
    float t1 = RaytraceFloor(ro, rd, floor_normal, floor_pos);

    // Check if floor was closet and in view of the camera.
    if (t1 < t0 && t1 >= camera_.near && t1 < camera_.far) {
        t = t1;
        p = ro + rd*t;
        normal = floor_normal;
        texture = GetFloorTexture(p);
    } else if (i < steps_ && t0 >= camera_.near && t0 < camera_.far) {
        t = t0;
        p = ro + rd*t;
        normal = GetNormal(p);
    } else {
        return sky_color_;
    }

    vec4 color;
    float z = mapTo(t, camera_.near, camera_.far, 1, 0);
    // Color as a function of distance
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
