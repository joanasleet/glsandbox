#version 440

out vec4 color;

in vec2 teTex;
in vec4 tePos;
in float height;

layout (binding = 0) uniform sampler2D samplerDiff;

void main() {

	//color = vec4(0.1, 0.1, 0.12, 1.0);
	float h = clamp(2*height, 2*height, 4*height);
	color = texture(samplerDiff, 50*teTex);
	color = vec4(h * color.xyz, 1.0); 
}