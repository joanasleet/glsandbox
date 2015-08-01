#version 440

layout(location = 0) in vec4 pos;

out vec3 texcoords;

uniform mat4 MVP;
uniform mat4 mv = mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1 );

void main() {
      
    texcoords = pos.xyz;

    gl_Position = MVP * mv * pos;
}
