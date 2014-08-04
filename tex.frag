#version 440

in vec2 texcoord;
uniform sampler2D sampler;

out vec4 finalColor;

void main() {
  
    finalColor = texture(sampler, texcoord);
}