#include "ParticlesRenderer.h"
#include <string>
#include <iostream>
#include "Utils/FileUtils.h"
#include "Utils/ShaderUtils.h"

ParticlesRenderer::ParticlesRenderer() : m_bufferIdx{ 0 }, m_computeProgramIdx{ 0 }, m_renderProgramIdx{ 0 }, m_vaoIdx{ 0 }
{
}

ParticlesRenderer::~ParticlesRenderer()
{
	cleanUp();
}

bool ParticlesRenderer::initializeShaders()
{

	std::string fragmentShaderSource;

	//TODO read from conf file
	std::string fragmentShaderFilePath = "C:/Users/Wiktor/projects/ParticlesSimulation/shaders/fragment_shader.glsl";
	if (!FileUtils::readFileToString(fragmentShaderFilePath, fragmentShaderSource))
	{
		return false;
	}

	std::string vertexShaderSource;
	std::string vertexShaderFilePath = "C:/Users/Wiktor/projects/ParticlesSimulation/shaders/vertex_shader.glsl";
	if (!FileUtils::readFileToString(vertexShaderFilePath, vertexShaderSource))
	{
		return false;
	}
	GLuint fragmentShader = ShaderUtils::compileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
	if (fragmentShader == 0) {
		return false;
	}
	GLuint vertexShader = ShaderUtils::compileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
	if (vertexShader == 0) {
		return false;
	}

	m_renderProgramIdx = ShaderUtils::createRenderProgram(vertexShader, fragmentShader);
	if (m_renderProgramIdx == 0)
	{
		return false;
	}

	std::string computeShaderSource;
	std::string computeShaderFilePath = "C:/Users/Wiktor/projects/ParticlesSimulation/shaders/compute_shader.glsl";
	if (!FileUtils::readFileToString(computeShaderFilePath, computeShaderSource))
	{
		return false;
	}
	GLuint computeShader = ShaderUtils::compileShader(computeShaderSource.c_str(), GL_COMPUTE_SHADER);
	if (computeShader == 0) {
		return false;
	}

	m_computeProgramIdx = ShaderUtils::createComputeProgram(computeShader);

	if (m_computeProgramIdx == 0) {
		return false;
	}

	return true;
}

void ParticlesRenderer::initializeParticles()
{
	m_particles.reserve(MAX_NUMBER_OF_PARTICLES);
	for (auto i = 0; i < MAX_NUMBER_OF_PARTICLES; i++) {
		Particle particle;
		particle.pos[0] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
		particle.pos[1] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
		particle.velocity[0] = 0.0f;
		particle.velocity[1] = 0.0f;
		particle.color[0] = 1.0f;
		particle.color[1] = 0.0f;
		particle.color[2] = 0.0f;
		particle.color[3] = 0.0f; // Alpha
		m_particles.push_back(particle);
	}
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
		<< " type = " << type
		<< ", severity = " << severity
		<< ", message = " << message << "\n";
}


void ParticlesRenderer::initializeGraphicsResources()
{
	glGenBuffers(1, &m_bufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferIdx);
	glBufferData(GL_ARRAY_BUFFER, m_particles.size() * sizeof(Particle), &m_particles[0], GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_vaoIdx);
	glBindVertexArray(m_vaoIdx);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(offsetof(Particle, color)));
	glEnableVertexAttribArray(1);
	//glPointSize(2.0f); // Set the point size to 10 pixels

	//glEnable(GL_DEBUG_OUTPUT);
	////glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback(MessageCallback, 0);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

}

bool ParticlesRenderer::initialize()
{
	if (!initializeShaders()) {
		return false;
	}
	initializeParticles();
	initializeGraphicsResources();
	return true;
}

void ParticlesRenderer::cleanUp()
{
	if (m_renderProgramIdx != 0) {
		glDeleteProgram(m_renderProgramIdx);
		m_renderProgramIdx = 0;
	}
	if (m_computeProgramIdx != 0) {
		glDeleteProgram(m_computeProgramIdx);
		m_computeProgramIdx = 0;
	}
	if (m_vaoIdx != 0) {
		glDeleteVertexArrays(1, &m_vaoIdx);
		m_vaoIdx = 0;
	}
	if (m_bufferIdx != 0) {
		glDeleteBuffers(1, &m_bufferIdx);
		m_bufferIdx = 0;
	}
}

void ParticlesRenderer::calculateFrame(const ParticleFrameData& frameData)
{
	glPointSize(frameData.particleSize); 

	glClear(GL_DEPTH_BUFFER_BIT);  

	glUseProgram(m_computeProgramIdx);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_bufferIdx);
	glUniform1f(glGetUniformLocation(m_computeProgramIdx, "deltaTime"), frameData.deltaTime);
	glUniform2f(glGetUniformLocation(m_computeProgramIdx, "mousePos"), (frameData.mousePosX / frameData.width) * 2.0f - 1.0f, 1.0f - (frameData.mousePosY / frameData.height) * 2.0f);
	glUniform1f(glGetUniformLocation(m_computeProgramIdx, "repulsionStrength"), 0.1f);
	glUniform1i(glGetUniformLocation(m_computeProgramIdx, "activeCount"), frameData.activeParticles);

	int workGroupSize = 12; 
	glDispatchCompute((frameData.activeParticles + workGroupSize - 1) / workGroupSize, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferIdx);

	glUseProgram(m_renderProgramIdx);
	glBindVertexArray(m_vaoIdx);
	glDrawArrays(GL_POINTS, 0, frameData.activeParticles);
}
