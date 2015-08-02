#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

out vec4 vPos;
out vec2 vTex;

void main() {

    vPos = pos;
    vTex = tex;

    gl_Position = vec4( pos, 1.0 );
}
