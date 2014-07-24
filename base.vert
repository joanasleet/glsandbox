#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec2 tex_coord;

out vec3 control_point;

uniform mat4 P, MV;

out vec2 texcoord;

void main() {
    
    gl_Position = P * MV * vertex_position;
    texcoord = tex_coord;

    control_point = vertex_position.xyz;
}