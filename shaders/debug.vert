#version 440

layout(location = 0) in vec4 pos;

uniform mat4 MVP;

void main() {

    gl_Position = MVP*pos;
}
