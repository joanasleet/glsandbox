#version 440

in vec4 vert_pos;
in vec4 tex_coord;

out vec4 finalColor;

uniform sampler2D sampler;

void main() {

    //finalColor = texture(sampler, vec2(tex_coord.x, 1-tex_coord.y)) - vec4(0.9, 0.9, 0.9, 0.0);
    //finalColor = texture(sampler, vec2(tex_coord.x, 1-tex_coord.y));

    float dist = distance(vert_pos.xy, vec2(0.0, 0.0));
    float delta = fwidth(dist);
    float alpha = smoothstep(0.45 - delta, 0.45, dist);
    finalColor = mix(vec4(0.1, 0.2, 0.3, 1.0), vec4(1.0), alpha);
}