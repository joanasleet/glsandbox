#version 440

in vec2 texcoord;

out vec4 finalColor;

layout ( binding = 0 ) uniform sampler2D s1;
layout ( binding = 1 ) uniform sampler2D s2;
layout ( binding = 2 ) uniform sampler2D s3;

void main() {

    finalColor = texture( s1, texcoord );
}
