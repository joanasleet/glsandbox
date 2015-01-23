#version 440

in float height;
in float maxHeight;

out vec4 color;

uniform sampler2D skymap;
uniform float gTime;

uniform float timeScale = 0.05;

void main() {

    color = vec4(0.0);

    if (height >= 0.0)
        color = texture(skymap, vec2(timeScale * gTime, 1.0 - height / (1.01 * maxHeight)));
}