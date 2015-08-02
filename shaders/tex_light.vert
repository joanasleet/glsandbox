#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

uniform mat4 P, MV;

out vec3 position_eye, normal_eye;
out vec2 vTex;

void main() {

    vTex = tex;
    position_eye = vec3( MV * vec4( pos, 1.0 ) );
    normal_eye = vec3( MV * vec4( normal, 0.0) );

    gl_Position = P * vec4(position_eye, 1.0);
}
