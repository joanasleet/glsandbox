#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec2 tex_coord;

//uniform mat4 P, MV;
uniform mat4 MVP;

out vec2 texcoord;

void main() {
    
    texcoord = tex_coord;

    //gl_Position = P * MV * vertex_position;
    gl_Position = MVP * vertex_position;
}