#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 mv; 
uniform mat4 MVP;

out vec3 vPos;
out vec2 vTex;

void main() {

    vPos = pos;
    vTex = tex;

    gl_Position = MVP * mv * vec4( pos, 1.0f );
}
