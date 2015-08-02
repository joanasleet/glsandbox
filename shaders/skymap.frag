#version 330

in vec4 tePos;
in float height;
in float maxHeight;

out vec4 color;

uniform float gTime;
uniform float timeScale = 0.01;
uniform sampler2D skymap;

void main() {

    /* sun position */
    float x = maxHeight * cos(timeScale * gTime) * cos(timeScale * gTime * 1);
    float y = maxHeight * cos(timeScale * gTime) * sin(timeScale * gTime * 1);
    float z = maxHeight * sin(timeScale * gTime);
    vec3 sunPos = vec3(x, z, -y);

    /* sky color */
    float u = step(1.0, z) * (z / (2.0 * maxHeight));
    float v = 1.0 - height / (1.01 * maxHeight);
    color = texture(skymap, vec2(u, (v)));

    float sunVertDist = distance(sunPos, tePos.xyz);

    /* sun size */
    float sunSize = 10.0;
    float sunProxim = (1.0 / sunSize) * sunVertDist;
    float sunAlpha = smoothstep(0.4, 0.5, sunProxim);

    /* sun color */
    vec4 sunColor = vec4(1.0, 1.0, 1.0, 1.0);
    sunColor = mix(sunColor, vec4(1.0, 0.9, 0.0, 1.0),  sunAlpha);

    /* sun glow */
    float glowSize = 2.0 * maxHeight;
    float glowAlpha = mix(1.0, 1.2, (1.0 - sunVertDist / glowSize));
    float glowSwitch = step(glowSize, sunVertDist);
    color = (1.0 - glowSwitch) * glowAlpha * color + glowSwitch * color;

    // blend sky with sun color
    color = mix(sunColor, color, sunAlpha);
}
