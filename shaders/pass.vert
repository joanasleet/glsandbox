#version 440

layout(location = 0) in vec4 pos;

out vec4 vPos;

void main() {

    gl_Position = pos;
    vPos = pos;
}