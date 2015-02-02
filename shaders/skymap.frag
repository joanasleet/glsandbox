#version 440

in vec4 tePos;
in float height;
in float maxHeight;

out vec4 color;

uniform sampler2D skymap;
uniform float gTime;

uniform float timeScale = 0.01;

void main() {

    /* sun position */
    float x = maxHeight * cos(timeScale * gTime) * cos(timeScale * gTime * 1);
    float y = maxHeight * cos(timeScale * gTime) * sin(timeScale * gTime * 1);
    float z = maxHeight * sin(timeScale * gTime);
    vec3 sunPos = vec3(x, z, -y);

    /* sky color */
    float u = step(0.0, z) * (z / (2.0 * maxHeight));
    float v = 1.0 - height / (1.01 * maxHeight);
    color = texture(skymap, vec2(u, v));

    float sunVertDist = distance(sunPos, tePos.xyz);

    /* sun size */
    float sunSize = 15.0;
    float sunProxim = (1.0 / sunSize) * sunVertDist;
    float sunAlpha = smoothstep(0.4, 0.5, sunProxim);

    /* sun color */
    vec4 sunColor = vec4(1.0, 1.0, 1.0, 1.0);
    sunColor = mix(sunColor, vec4(1.0, 0.9, 0.0, 1.0),  sunAlpha);

    /* rise/dawn color */
    vec4 riseColor = vec4(1.0, 0.8, 0.8, 1.0);
    sunColor = mix(sunColor, vec4(1.0, 0.2, 0.0, 1.0), 1.0 - min(1.0, z * z / maxHeight));

    /* sun glow */
    float glowSize = 2.0 * maxHeight;
    float glowAlpha = mix(1.0, 3.0, (1.0 - sunVertDist / glowSize));
    float glowSwitch = step(glowSize, sunVertDist);
    color = (1.0 - glowSwitch) * glowAlpha * color + glowSwitch * color;

    // blend sky with sun color
    color = mix(sunColor, color, sunAlpha);

}