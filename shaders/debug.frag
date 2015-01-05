#version 440

out vec4 finalColor;

in vec2 teTex;
in float height;

layout (binding = 0) uniform sampler2D samplerDiff;

void main() {

	//finalColor = vec4(0.1, 0.1, 0.12, 1.0);
	finalColor = texture(samplerDiff, 50*teTex);
	finalColor = vec4(5 * height * finalColor.xyz, 1.0); 
}