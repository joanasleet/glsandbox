#version 440

layout(location = 0) in vec4 coords;

out vec3 vs_tex_coords;

uniform mat4 MVP;

void main() {
    
    gl_Position = MVP * coords;
    
    vs_tex_coords = coords.xyz;
}