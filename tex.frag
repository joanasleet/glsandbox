#version 440

in vec3 texcoord;
uniform sampler2D sampler;

uniform mat4 MV;

out vec4 finalColor;

void main() {
       
    finalColor = texture(sampler, texcoord);
}