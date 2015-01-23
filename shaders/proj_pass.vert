#version 440

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 tex_coord;

uniform mat4 MVP;

out vec2 texcoord;
out vec4 vPos;

void main() {

    texcoord = tex_coord;
    vPos = pos;

    gl_Position = MVP * pos;
}