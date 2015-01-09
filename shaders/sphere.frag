#version 440

in vec2 teTex;
in vec4 tePos;
in float height;

out vec4 color;

void main() {

	// sun
	float sun_size = 50.0;
	vec2 sun_pos = vec2(0.0, 0.0);
	vec4 sunColor =vec4(0.8, 0.5, 0.0, 1.0);

	// sky
	vec4 skyColor = vec4(0.1, 0.7, 0.9, 1.0);

	float dist = (1.0/sun_size) * distance(tePos.xy, sun_pos);
    float delta = 5.0/sun_size;
	float alpha = smoothstep(0.45-delta, 0.45, dist);
	color = mix(sunColor, skyColor, alpha);
}