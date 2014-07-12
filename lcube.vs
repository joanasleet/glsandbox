#version 440

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec3 vertex_normal;

uniform mat4 P, MV;

out vec3 position_eye, normal_eye;

out vec3 cube_texel;

void main() {

    position_eye = vec3( MV * vertex_position );
    normal_eye = vec3( MV * vec4(vertex_normal, 0.0) );
    gl_Position = P * vec4(position_eye, 1.0);

    cube_texel = vertex_position.xyz;
    
    /*
    gl_Position = P * MV * coords;
    
    vs_tex_coords = coords.xyz;

    vec4 neye = MV * vec4(normals, 1.0);
    vs_normal_eye = neye.xyz;

    vec4 peye = MV * coords;
    vs_position_eye = peye.xyz;
    */
    
}