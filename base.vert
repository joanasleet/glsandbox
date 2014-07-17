#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 vertex_normal;

uniform mat4 P, MV;

out vec3 texcoord;

void main() {
    
    gl_Position = P * MV * vertex_position;
    texcoord = tex_coord;
}