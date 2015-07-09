#version 440

in vec4 vsColor;
in vec4 vsSecColor;
in vec3 vsLightPos;
in vec3 vsDirection;

out vec4 color;

const float g = -0.99;
const float g2 = g * g;

vec4 skyColor() {
    float fCos = dot(vsLightPos, vsDirection) / length(vsDirection);
    float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos * fCos) / pow(1.0 + g2 - 2.0 * g * fCos, 1.5);

    return ( vsColor + fMiePhase*vsSecColor );
}

void main() {

    color = skyColor();
}
