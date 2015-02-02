#version 440

out vec4 color;

uniform sampler2D skymap;
uniform float gTime;

uniform float timeScale = 0.05;

void main() {

    color = vec4(0.0, 0.4, 0.9, 0.55);
}