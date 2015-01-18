#version 440

in vec2 teTex;
in vec4 tePos;

in float height;
in float rad;

out vec4 color;

//uniform float time;
const float sunSize = 5.0;

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
    //fcolor.a = fcolor.b;
    return fcolor;
}

void main() {

    // sky
    color = skyColor();

    /* ~ ~ ~ ~ ~ ~ sun ~ ~ ~ ~ ~ ~ ~ */
    vec4 sunColor = vec4(1.0, 1.0, 0.2, 1.0);
    vec4 sunInnerColor = vec4(1.0, 1.0, 1.0, 1.0);
    float dist = (1.0 / sunSize) * distance(v3LightPos, tePos.xyz);
    float delta = 0.6;
    float edge = 1.0;
    float alpha = smoothstep(edge - delta, edge, dist);
    sunColor = mix(sunInnerColor, sunColor, alpha);
    /* ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ */

    /* mix sky and sun */
    //color = mix(sunColor, color, alpha);
}