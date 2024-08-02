#version 430
layout (local_size_x = 1024) in; 

struct Particle {
    vec2 pos;
    vec2 vel;
    vec4 color;
};

layout (std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform int activeCount;  
uniform float deltaTime;
uniform vec2 mousePos;
uniform float repulsionStrength;


void main() {
    uint idx = gl_GlobalInvocationID.x; 
    if (idx >= activeCount) return; 


    vec2 difference = particles[idx].pos - mousePos;
    float distance = length(difference);
    if (distance < 0.0001) distance = 0.0001; 

    float forceMagnitude = repulsionStrength / (distance * distance);
    vec2 normDifference = normalize(difference);
    vec2 repulsionForce = normDifference * forceMagnitude;

    particles[idx].vel += repulsionForce * deltaTime;

    particles[idx].vel *= pow(0.94, deltaTime);

    particles[idx].pos += particles[idx].vel * deltaTime;
    if (particles[idx].pos.x <= -1.0 || particles[idx].pos.x >= 1.0) {
        particles[idx].vel.x *= -1.0; 
    }
    if (particles[idx].pos.y <= -1.0 || particles[idx].pos.y >= 1.0) {
        particles[idx].vel.y *= -1.0;
    }
    
	vec3 baseColor = vec3(1.0, 1.0, 1.0); 
	float speed = length(particles[idx].vel);
	particles[idx].color.rgb = mix(baseColor, vec3(1.0, 0.0, 0.0), clamp(speed, 0.0, 1.0));
	particles[idx].color.a = 1.0; 
}
