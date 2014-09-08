#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec2 tex_coord;

//uniform mat4 P, MV;
//uniform mat4 MVP;

void main() {
    
    float n = 3.0;
    float p = 1/n;

    vec3 pos = vertex_position.xyz;
    pos.x *= p;
    pos.x += (1-p); 
    
    gl_Position = vec4(pos, 1.0);
}