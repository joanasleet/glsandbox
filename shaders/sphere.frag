#version 440

in vec2 teTex;
in vec4 tePos;

uniform sampler2D samplerDiff;

out vec4 color;

void main() {

	//color = vec4(0.1, 0.1, 0.12, 1.0);
	//color = texture(samplerDiff, teTex);
	color = tePos; 
}