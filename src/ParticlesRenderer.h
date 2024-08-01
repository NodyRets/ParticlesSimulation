#pragma once
#include <glad/gl.h>
#include "Particle.h"
#include <vector>
#include "ParticleFrameData.h"

class ParticlesRenderer
{
public:

	static constexpr auto MAX_NUMBER_OF_PARTICLES = 50000000;

	ParticlesRenderer();
	~ParticlesRenderer();

	bool initialize();
	void cleanUp();

	void calculateFrame(const ParticleFrameData& frameData);

private:

	bool initializeShaders();
	void initializeParticles();
	void initializeGraphicsResources();

	std::vector<Particle> m_particles;
	GLuint m_renderProgramIdx;
	GLuint m_computeProgramIdx;
	GLuint m_vaoIdx;
	GLuint m_bufferIdx;
};