#version 330

layout(location = 0) in vec3 pos;

out vec3 vTex;

uniform mat4 MVP;
uniform mat4 mv;

void main() {
      
    vTex = pos.xyz;

    gl_Position = MVP * mv * vec4( pos, 1.0 );
}
