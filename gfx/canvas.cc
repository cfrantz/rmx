#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE
#include "gfx/canvas.h"

#include "absl/memory/memory.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/io.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"

namespace GFX {
namespace {
const char kVertexShader[] = R"shader(
#version 330 core
uniform mat4 projection_matrix;
in vec2 position;
in vec2 uv;
in vec4 color;
out vec2 frag_uv;
out vec4 frag_color;
void main() {
    frag_uv = uv;
    frag_color = color;
    gl_Position = projection_matrix * vec4(position.xy, 0, 1);
}
)shader";

const char kFragmentShader[] = R"shader(
#version 330 core
uniform sampler2D frag_texture;
in vec2 frag_uv;
in vec4 frag_color;
out vec4 out_color;
void main() {
    out_color = frag_color * texture(frag_texture, frag_uv.st);
}
)shader";
}  // namespace

std::unique_ptr<Context2D> Context2D::New() {
    return absl::make_unique<Context2D>(Shader::New(kVertexShader,
                                                    kFragmentShader));
}

void Context2D::AddLine(const glm::vec2& a,
                        const glm::vec4& colora,
                        const glm::vec2& b,
                        const glm::vec4& colorb,
                        float thickness) {
    glm::vec2 halfunit = glm::normalize(b - a) * thickness * 0.5f;
    glm::vec2 n(halfunit.y, -halfunit.x);

    uint32_t vtx = vertices_.size();
    AddVertex(a-n, glm::vec2(0, 0), colora);
    AddVertex(b-n, glm::vec2(0, 0), colorb);
    AddVertex(b+n, glm::vec2(0, 0), colorb);
    AddVertex(a+n, glm::vec2(0, 0), colora);

    indices_.push_back(vtx + 0);
    indices_.push_back(vtx + 1);
    indices_.push_back(vtx + 2);
    indices_.push_back(vtx + 0);
    indices_.push_back(vtx + 2);
    indices_.push_back(vtx + 3);
    command_.back().count += 6;
}

void Context2D::AddLine(const glm::vec2& a,
                        const glm::vec2& b,
                        const glm::vec4& color,
                        float thickness) {
    AddLine(a, color, b, color, thickness);
}

void Context2D::AddPolyline(const std::vector<glm::vec2>& points,
                            const glm::vec4& color, bool closed,
                            float thickness) {
    const glm::vec2 zero(0, 0);
    std::vector<glm::vec2> normal(points.size() - (closed ? 0 : 1));
    size_t i;
    // Compute normals for each line segment.
    for(i=0; i<points.size()-1; ++i) {
        glm::vec2 unit = points[i+1] - points[i];
        if (unit != zero) unit = glm::normalize(unit);
        normal[i] = glm::vec2(unit.y, -unit.x);
    }
    if (closed) {
        glm::vec2 unit = points[0] - points[i];
        if (unit != zero) unit = glm::normalize(unit);
        normal[i] = glm::vec2(unit.y, -unit.x);
    }

    // For a closed path, compute the normal average for all points.
    // For an open path, compute the normal average for all non-endpoints.
    std::vector<glm::vec2> avg(points.size());
    if (closed) {
        avg[0] = (normal[0] + normal[normal.size()-1]) * 0.5f;
        for(i=1; i<normal.size(); ++i) {
            avg[i] = (normal[i-1] + normal[i]) * 0.5f;
        }
    } else {
        avg[0] = normal[0];
        avg[avg.size()-1] = normal[normal.size()-1];
        for(i=1; i<normal.size(); ++i) {
            avg[i] = (normal[i-1] + normal[i]) * 0.5f;
        }
    }
    thickness *= 0.5f;
    for(auto& a : avg) {
        float r2 = a.x*a.x + a.y*a.y;
        float scale = 1.0f;
        if (r2 > 0.00001f) {
            scale = 1.0f/r2;
            if (scale > 100.0f) scale = 100.0f;
        }
        a *= scale * thickness;
    }

    uint32_t vtx1 = vertices_.size();
    if (!closed) {
        const auto& p = points[0];
        const auto& n = avg[0];
        AddVertex(p-n, zero, color);
        AddVertex(p+n, zero, color);
    } else {
        vtx1 += (points.size() - 1) * 2;
    }

    for(i=0; i<points.size() - (closed ? 0 : 1); ++i) {
        size_t j = (i + 1) == points.size() ? 0 : (i + 1);
        const auto& p = points[j];
        const auto& n = avg[j];
        uint32_t vtx2 = vertices_.size();

        AddVertex(p-n, zero, color);
        AddVertex(p+n, zero, color);
        indices_.push_back(vtx1 + 0);
        indices_.push_back(vtx2 + 0);
        indices_.push_back(vtx1 + 1);
        indices_.push_back(vtx1 + 1);
        indices_.push_back(vtx2 + 0);
        indices_.push_back(vtx2 + 1);

        vtx1 = vtx2;
        vtx2 = vertices_.size();
        command_.back().count += 6;
    }
}

void Context2D::AddPrimitiveTriangle(
        const glm::vec2& a,
        const glm::vec4& colora,
        const glm::vec2& b,
        const glm::vec4& colorb,
        const glm::vec2& c,
        const glm::vec4& colorc) {

    uint32_t vtx = vertices_.size();
    AddVertex(a, glm::vec2(0, 0), colora);
    AddVertex(b, glm::vec2(0, 0), colorb);
    AddVertex(c, glm::vec2(0, 0), colorc);
    indices_.push_back(vtx++);
    indices_.push_back(vtx++);
    indices_.push_back(vtx++);
    command_.back().count += 3;
}

void Context2D::AddPrimitiveTriangle(
        const glm::vec2& a,
        const glm::vec2& b,
        const glm::vec2& c,
        const glm::vec4& color) {
    AddPrimitiveTriangle(a, color, b, color, c, color);
}

void Context2D::Draw() {

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    //glEnable(GL_SCISSOR_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, fb_width_, fb_height_);
    glm::mat4 projection = glm::ortho(0.0f, float(fb_width_),
                                      float(fb_height_), 0.0f);
    shader_->Use();
    glUniformMatrix4fv(vars_.projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glBindSampler(0, 0); // Rely on combined texture/sampler state.

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glEnableVertexAttribArray(vars_.position);
    glEnableVertexAttribArray(vars_.uv);
    glEnableVertexAttribArray(vars_.color);
    glVertexAttribPointer(vars_.position, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
    glVertexAttribPointer(vars_.uv, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
    glVertexAttribPointer(vars_.color, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
                 (const GLvoid*)vertices_.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint),
                 (const GLvoid*)indices_.data(), GL_STREAM_DRAW);
    GLuint* offset = 0;
    for(const auto& c : command_) {
        if (c.antialias) {
            glEnable(GL_MULTISAMPLE);
        } else {
            glDisable(GL_MULTISAMPLE);
        }
        glBindTexture(GL_TEXTURE_2D, c.texture_id);
        //glScissor((int)pcmd->ClipRect.x,
        //          (int)(fb_height - pcmd->ClipRect.w),
        //          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
        //          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
        glDrawElements(GL_TRIANGLES, c.count, GL_UNSIGNED_INT, offset);
        offset += c.count;
    }
}

void Context2D::Clear() {
    vertices_.clear();
    indices_.clear();
    command_.clear();
    InitCommandList();
}

void Context2D::Translate(const glm::vec2& t) {
    transform_ = glm::translate(transform_, t);
    std::cout << "Translate: " << transform_ << std::endl;
}
void Context2D::Rotate(float a) {
    transform_ = glm::rotate(transform_, a);
    std::cout << "Rotate: " << transform_ << std::endl;
}
void Context2D::Scale(const glm::vec2& s) {
    transform_ = glm::scale(transform_, s);
}
void Context2D::ShearX(float s) {
    transform_ = glm::shearX(transform_, s);
}
void Context2D::ShearY(float s) {
    transform_ = glm::shearY(transform_, s);
}

void Context2D::AddVertex(const glm::vec2& pos, const glm::vec2& uv, const glm::vec4& color) {
    //glm::vec2 p((glm::vec3(pos, 1.0f) * transform_).xy);
    glm::vec3 a(pos, 1.0f);
    glm::vec3 b(transform_ * a);
    glm::vec2 p(b.xy);

    std::cout << "AddVertex: " << a << " -> " << b << " -> " << p << std::endl;

    vertices_.emplace_back(Vertex{p, uv, color});
    //std::cout << "AddVertex: " << p << " -> " << vertices_.back().pos << std::endl;
}

void Context2D::Init() {
    vars_.projection_matrix = glGetUniformLocation(shader_->program(),
                                                   "projection_matrix");
    vars_.position = glGetAttribLocation(shader_->program(), "position");
    vars_.uv = glGetAttribLocation(shader_->program(), "uv");
    vars_.color = glGetAttribLocation(shader_->program(), "color");
    vars_.frag_texture = glGetUniformLocation(shader_->program(),
                                              "frag_texture");

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    InitWhitePixel();
    InitCommandList();
}

void Context2D::InitWhitePixel() {
    uint32_t white_image = 0xFFFFFFFF;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &white_pixel_);
    glBindTexture(GL_TEXTURE_2D, white_pixel_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, (void*)&white_image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Context2D::InitCommandList() {
    command_.emplace_back(VertexCommand{white_pixel_,
                                        antialias_,
                                        0});
}


}  // namespace GFX
