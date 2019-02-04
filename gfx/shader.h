#ifndef CANVAS_GFX_SHADER_H
#define CANVAS_GFX_SHADER_H

#include <string>
#include <memory>
#include <GL/glew.h>
#include "util/status.h"

namespace GFX {

class Shader {
  public:
    static std::unique_ptr<Shader> New(const char* vs, const char* fs,
                                       const char* gs=nullptr);
    static std::unique_ptr<Shader> Load(const std::string& vs,
                                        const std::string& fs,
                                        const std::string& gs);
    inline void Use() const { glUseProgram(program_); }
    inline GLuint program() const { return program_; }
  private:
    Shader(const char* vs, const char* fs, const char* gs);
    void CheckCompileErrors(GLuint shader, const std::string& type);
    static util::Status ProcessIncludes(std::string* text);

    GLuint program_;
};

}  // namespace GFX
#endif // CANVAS_GFX_SHADER_H
