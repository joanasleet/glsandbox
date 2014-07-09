#version 440

layout(location = 0) in vec4 coords;
layout(location = 1) in vec2 tex_coords;

out vec2 vs_tex_coords;

uniform mat4 MVP;

void main() {
    
    gl_Position = MVP * coords;
    
    vs_tex_coords = tex_coords;
}