#version 440

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 tex;

out vec4 vPos;
out vec2 texcoord;

void main() {

    gl_Position = pos;
    vPos = pos;
    texcoord = tex;
}
