// Compute shader
#version 430
layout (local_size_x = 12) in; // Define the number of threads in a workgroup

struct Particle {
    vec2 pos;
    vec2 vel;
    vec4 color;
};

layout (std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform int activeCount;  // Number of active particles to process
uniform float deltaTime;
uniform vec2 mousePos;
uniform float repulsionStrength;


void main() {
    uint idx = gl_GlobalInvocationID.x; // Get the index of the current thread
    if (idx >= activeCount) return; // Safety check


    vec2 difference = particles[idx].pos - mousePos;
    float distance = length(difference);
    if (distance < 0.0001) distance = 0.0001; // Avoid division by zero

    float forceMagnitude = repulsionStrength / (distance * distance);
    vec2 normDifference = normalize(difference);
    vec2 repulsionForce = normDifference * forceMagnitude;

    particles[idx].vel += repulsionForce * deltaTime;

     // Apply damping to reduce the velocity
    particles[idx].vel *= pow(0.94, deltaTime);

    particles[idx].pos += particles[idx].vel * deltaTime;
    if (particles[idx].pos.x <= -1.0 || particles[idx].pos.x >= 1.0) {
        particles[idx].vel.x *= -1.0; // Reverse horizontal velocity
    }
    if (particles[idx].pos.y <= -1.0 || particles[idx].pos.y >= 1.0) {
        particles[idx].vel.y *= -1.0; // Reverse vertical velocity
    }
    // Example of dynamic color based on speed
	vec3 baseColor = vec3(1.0, 1.0, 1.0); // Some base color
	float speed = length(particles[idx].vel);
	particles[idx].color.rgb = mix(baseColor, vec3(1.0, 0.0, 0.0), clamp(speed, 0.0, 1.0));
    //particles[idx].color.rgb = mix(vec3(0, 0, 1), vec3(1, 0, 0), clamp(speed / 5, 0.0, 1.0));

	particles[idx].color.a = 1.0; // Full opacity}
}
