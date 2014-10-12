#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 vertex_normal;

uniform mat4 P, MV;

out vec3 position_eye, normal_eye;

out vec2 texcoord;

void main() {

    position_eye = vec3( MV * vertex_position );
    normal_eye = vec3( MV * vec4(vertex_normal, 0.0) );
    gl_Position = P * vec4(position_eye, 1.0);

    texcoord = tex_coord;
}