#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec2 tex_coord;

//uniform mat4 P, MV;
//uniform mat4 MVP;

uniform float xtransform = 1;
uniform float ytransform = 1;
uniform float ztransform = 1;

out vec2 texcoord;

float transformC(float t, float coord) {
    return ( (1/t)*(coord-1)+1 );
}

void main() {

    texcoord = tex_coord;
    
    vec3 pos = vertex_position.xyz;
    pos.x = transformC(xtransform, pos.x);
    pos.y = transformC(ytransform, pos.y);
    pos.z = transformC(ztransform, pos.z);
    
    gl_Position = vec4(pos, 1.0);
}


