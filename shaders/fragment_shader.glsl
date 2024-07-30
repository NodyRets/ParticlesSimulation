#version 430 core
in vec4 fragColor;
out vec4 fragment;

void main() {
        vec2 center = vec2(0.5, 0.5);
    float radius = 0.5;
    float dist = distance(gl_PointCoord, center);
    if (dist > radius) {
        discard; // Discard fragments that are outside the radius of the circle
    }

    float alpha = 1.0 - smoothstep(0.45, 0.5, dist);
    fragment = vec4(fragColor.rgb, alpha * fragColor.a);}