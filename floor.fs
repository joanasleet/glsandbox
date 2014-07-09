#version 440

in vec2 vs_tex_coords;

out vec4 color;

uniform sampler2D sampler;

void main() {
   
    color = texture(sampler, vs_tex_coords);
}