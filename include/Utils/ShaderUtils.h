#pragma once
#include <glad/gl.h>

namespace ShaderUtils {

	GLuint compileShader(const char* source, GLenum type);
	GLuint createRenderProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint createComputeProgram(GLuint computeShader);
	const char* shaderTypeToString(GLenum type);

}