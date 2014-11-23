#version 440

in vec4 tex_coord;
out vec4 finalColor;

uniform sampler2D sampler;

void main() {
  
    //finalColor = vec4(0.1, 0.1, 0.1, 0.7);
    //finalColor = texture(sampler, vec2(tex_coord.x, 1-tex_coord.y)) - vec4(0.9, 0.9, 0.9, 0.0);
    finalColor = texture(sampler, vec2(tex_coord.x, 1-tex_coord.y));
}