#version 440

in vec2 teTex;
in vec4 tePos;
in float height;
in float rad;

out vec4 color;

uniform sampler2D gmap;
uniform float time;

void main() {


	color = vec4(0.0);

	if(tePos.y > 0.0)
		color = texture(gmap, vec2(0.0, 1.0-float(tePos.y)/(1.1*rad) ));

	// sun
	float sun_size = 50.0;

	vec3 sun_pos = vec3(0.0, 0.0, -rad);
	float time_scale = 2.0;
	//sun_pos += vec3(time_scale * time, 10.0*sin(time_scale * time), 0.0);
	sun_pos += vec3(time_scale * time, 0.0, 0.0);

	vec4 sunColor =vec4(0.8, 0.5, 0.0, 1.0);
	float dist = (1.0/sun_size) * distance(tePos.xyz, sun_pos);
    float delta = 5.0/sun_size;
	float alpha = smoothstep(0.45-delta, 0.45, dist);
	vec4 finalsun = mix(sunColor, color, alpha);
	color += finalsun;
}