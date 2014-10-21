#version 440

layout(location = 0) in vec4 vertex_position;

uniform mat4 MVP;

void main() {
    
    gl_Position = MVP * vertex_position;
}