#version 440

in vec3 vs_tex_coords;

out vec4 color;

uniform samplerCube sampler;

void main() {
   
    color = texture(sampler, vs_tex_coords);
}