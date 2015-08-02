#version 330

in vec2 vTex;

out vec4 finalColor;

uniform sampler2D s1;

void main() {

    finalColor = texture( s1, vTex );
}
