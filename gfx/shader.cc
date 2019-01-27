#include "gfx/shader.h"

#include "absl/memory/memory.h"
#include "util/file.h"
#include "util/logging.h"

namespace GFX {

std::unique_ptr<Shader> Shader::New(const char* vs, const char* fs,
                                    const char* gs) {
    return absl::WrapUnique(new Shader(vs, fs, gs));
}

std::unique_ptr<Shader> Shader::Load(const std::string& vs,
                                     const std::string& fs,
                                     const std::string& gs) {
    std::string vertex, fragment, geometry;

    File::GetContents(vs, &vertex);
    File::GetContents(fs, &fragment);
    if (!gs.empty()) {
        File::GetContents(gs, &geometry);
    }

    return absl::WrapUnique(
            new Shader(vertex.c_str(), fragment.c_str(),
                       gs.empty() ? nullptr : geometry.c_str()));
}

Shader::Shader(const char* vs, const char* fs, const char* gs) {
    GLuint vertex = 0, fragment = 0, geometry = 0;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs, nullptr);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "vertex");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs, nullptr);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "fragment");

    if (gs) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gs, nullptr);
        glCompileShader(geometry);
        CheckCompileErrors(geometry, "geometry");
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vertex);
    glAttachShader(program_, fragment);
    if (geometry) {
        glAttachShader(program_, geometry);
    }
    glLinkProgram(program_);
    CheckCompileErrors(program_, "program");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometry) {
       glDeleteShader(geometry);
    }
}

void Shader::CheckCompileErrors(GLuint shader, const std::string& type) {
    GLint success = true;
    GLchar info[1024] = {0,};

    if (type == "program") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
    } else {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
    }
    if (!success) {
        glGetProgramInfoLog(shader, 1024, nullptr, info);
        LOG(ERROR, "Error in ", type, "(", shader, "):\n", info);
    }
}

}  // namespace GFX
