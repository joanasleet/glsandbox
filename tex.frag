#version 440

in vec2 texcoord;
uniform sampler2D sampler;

uniform mat4 MV;

out vec4 finalColor;

void main() {
  
    finalColor = texture(sampler, texcoord);
    finalColor = vec4(1.0, 1.0, 1.0, 1.0);
}