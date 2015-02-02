#version 440

out vec4 color;

in vec2 texcoord;
in float height;

layout (binding = 0) uniform sampler2D samplerDiff;

void main() {

    float h = 0.50 * clamp(height, 1.0, 3.0);
    color = texture(samplerDiff, 25.0 * texcoord);
    color = vec4(h * color.xyz, 1.0);
}