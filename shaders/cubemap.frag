#version 330

in vec3 vTex;

out vec4 color;

uniform samplerCube sampler;

void main() {
   
    color = texture( sampler, vTex );
}
