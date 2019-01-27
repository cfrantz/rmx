#ifndef CANVAS_GFX_CANVAS_H
#define CANVAS_GFX_CANVAS_H
#include <vector>
#include <memory.h>
#include <GL/glew.h>

#include "gfx/shader.h"
#include "glm/glm.hpp"

namespace GFX {

class Context2D {
  public:
    struct Vertex {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
    };
    struct VertexCommand {
        GLuint texture_id;
        bool antialias;
        size_t count;
    };
    static std::unique_ptr<Context2D> New();
    explicit Context2D(std::unique_ptr<Shader> shader)
      : shader_(std::move(shader)),
        antialias_(true),
        transform_(1.0f)
      { Init(); }


    void AddLine(
            const glm::vec2& a,
            const glm::vec2& b,
            const glm::vec4& color,
            float thickness=1.0f);

    void AddLine(
            const glm::vec2& a,
            const glm::vec4& colora,
            const glm::vec2& b,
            const glm::vec4& colorb,
            float thickness=1.0f);

    void AddPolyline(
            const std::vector<glm::vec2>& points,
            const glm::vec4& color,
            bool closed=true,
            float thickness=1.0f);

    void AddPrimitiveTriangle(
            const glm::vec2& a,
            const glm::vec2& b,
            const glm::vec2& c,
            const glm::vec4& color);

    void AddPrimitiveTriangle(
            const glm::vec2& a,
            const glm::vec4& colora,
            const glm::vec2& b,
            const glm::vec4& colorb,
            const glm::vec2& c,
            const glm::vec4& colorc);


    void SetViewport(GLuint w, GLuint h) { fb_width_ = w; fb_height_ = h; }
    void SetAntialias(bool aa) { antialias_ = aa; InitCommandList(); }
    void ResetTransform() { transform_ = glm::fmat3(1.0f); }
    void SetTransform(const glm::fmat3& t) { transform_ = t; }
    void Translate(const glm::vec2& t);
    void Rotate(float a);
    void Scale(const glm::vec2& s);
    void ShearX(float s);
    void ShearY(float s);

    void Draw();
    void Clear();
  private:
    void Init();
    void InitWhitePixel();
    void InitCommandList();
    void AddVertex(const glm::vec2& pos, const glm::vec2& uv,
                   const glm::vec4& color);

    std::unique_ptr<Shader> shader_;
    bool antialias_;
    glm::fmat3 transform_;

    // variables in the shader programs.
    struct {
        int projection_matrix;
        int position;
        int uv;
        int color;
        int frag_texture;
    } vars_;

    GLuint vao_;  // vertex array object
    GLuint vbo_;  // vertex buffer object
    GLuint ebo_;  // element buffer object
    GLuint white_pixel_;  // default texture

    GLuint fb_width_;
    GLuint fb_height_;

    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;
    std::vector<VertexCommand> command_;
};


}  // namespace GFX

#endif // CANVAS_GFX_CANVAS_H
