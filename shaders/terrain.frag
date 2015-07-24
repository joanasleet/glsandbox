#version 440

in vec3 geoCol;
out vec4 color;

in vec2 teUVg;
in vec3 tePos;
in float height;

layout (binding = 1) uniform sampler2D cliff;

void main() {

    color = texture( cliff, 20.0*teUVg );
    color = vec4( sqrt( height )*color.xyz, 1.0 );
}
