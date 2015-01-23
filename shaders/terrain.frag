#version 440

out vec4 color;

in vec2 texcoord;
in float height;

layout (binding = 0) uniform sampler2D samplerDiff;

void main() {

    float h = clamp(2.0 * height, 2.0 * height, 4.0 * height);
    color = texture(samplerDiff, 50.0 * texcoord);
    color = vec4(h * color.xyz, 1.0);
}