#version 440

in vec3 texcoords;

out vec4 color;

uniform samplerCube sampler;

void main() {
   
    color = texture(sampler, texcoords);
}
