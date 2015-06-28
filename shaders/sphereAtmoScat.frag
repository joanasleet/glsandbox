#version 440

in vec2 teTex;
in vec4 tePos;

in float height;
in float rad;

out vec4 color;

//uniform float time;

in vec4 teColor;
in vec4 teSecColor;

in vec3 v3LightPos;
const float g = -0.99;
const float g2 = g * g;

in vec3 v3Direction;

vec4 skyColor() {
    float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
    float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos * fCos) / pow(1.0 + g2 - 2.0 * g * fCos, 1.5);
    vec4 fcolor = teColor + fMiePhase * teSecColor;
    //fcolor.a = fcolor.b; // makes atmosphere grey and ugly
    return fcolor;
}

void main() {

    color = skyColor();
}
