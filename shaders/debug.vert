#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec4 texcoord;

uniform mat4 MVP;

out vec4 tex_coord;

void main() {
    
    //gl_Position = MVP * vertex_position;
    gl_Position = vertex_position;

    tex_coord = texcoord;
}