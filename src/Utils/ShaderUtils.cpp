#include "Utils/ShaderUtils.h"
#include "spdlog/spdlog.h"

namespace ShaderUtils {

    GLuint compileShader(const char* source, GLenum type)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            spdlog::error("[Shaders::compileShader] {} shader compilation failed: {}", shaderTypeToString(type), infoLog);
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    GLuint createProgram(GLuint shader1, GLuint shader2 = 0)
    {
        GLuint program = glCreateProgram();
        glAttachShader(program, shader1);
        if (shader2) glAttachShader(program, shader2);
        glLinkProgram(program);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
            spdlog::error("[Shaders::createProgram] Program linking failed: {}", infoLog);
            glDeleteProgram(program);
            program = 0;
        }

        glDeleteShader(shader1);
        if (shader2) {
            glDeleteShader(shader2);
        }

        return program;
    }

    GLuint createRenderProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        return createProgram(vertexShader, fragmentShader);
    }

    GLuint createComputeProgram(GLuint computeShader)
    {
        return createProgram(computeShader);
    }

    const char* shaderTypeToString(GLenum type)
    {
        switch (type) {
        case GL_VERTEX_SHADER: return "Vertex";
        case GL_FRAGMENT_SHADER: return "Fragment";
        case GL_GEOMETRY_SHADER: return "Geometry";
        case GL_TESS_CONTROL_SHADER: return "Tessellation Control";
        case GL_TESS_EVALUATION_SHADER: return "Tessellation Evaluation";
        case GL_COMPUTE_SHADER: return "Compute";
        default: return "Unknown";
        }
    }
}
