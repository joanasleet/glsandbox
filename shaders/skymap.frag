#version 440

in vec4 tePos;
in float height;
in float maxHeight;

out vec4 color;

uniform sampler2D skymap;
uniform float gTime;

uniform float timeScale = 0.05;

void main() {

    /* sun position */
    float x = maxHeight * cos(timeScale * gTime) * cos(timeScale * gTime * 0);
    float y = maxHeight * cos(timeScale * gTime) * sin(timeScale * gTime * 0);
    float z = maxHeight * sin(timeScale * gTime);
    vec3 sunPos = vec3(x, z, -y);

    /* sun color */
    vec4 sunColor = vec4(1.0, 0.9, 0.6, 1.0);
    vec4 riseColor = vec4(1.0, 0.2, 0.0, 1.0);
    float sunSize = 20.0;
    float dist = (1.0 / sunSize) * distance(sunPos, tePos.xyz);
    float delta = 0.2;
    float alpha = smoothstep(0.45 - delta, 0.45, dist);

    float u = (z / maxHeight);
    if (z < -dist) {
        //    u = 0.0;
    }
    vec2 uv = vec2(u, 1.0 - height / (1.01 * maxHeight));

    /* sky color */
    color = texture(skymap, uv);

    /* draw sun */
    sunColor = mix(riseColor, sunColor, 2.0 * sunPos.y / maxHeight);
    color = mix(sunColor, color, alpha);
}