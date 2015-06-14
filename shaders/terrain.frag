#version 440

in vec3 geoCol;
out vec4 color;

in vec2 teUVg;
in vec3 tePos;
in float height;

layout (binding = 1) uniform sampler2D cliff;
layout (binding = 2) uniform sampler2D grass;
layout (binding = 3) uniform sampler2D nmap;

mat3 align = mat3(
    1.0, 0.0, 0.0,
    0.0, cos(1.572), -sin(1.572),
    0.0, sin(1.572), cos(1.572)
);

void main() {


    vec3 norm = texture( nmap, teUVg ).xyz;
    norm = align * norm;

    vec3 blending = abs( norm );
    blending = normalize( max( blending, 0.00001 ) );
    float b = ( blending.x + blending.y + blending.z );
    blending /=  b;

    vec4 x = texture( cliff, tePos.yz );
    vec4 y = texture( grass, tePos.xz );
    vec4 z = texture( cliff, tePos.xy );

    color = x * blending.x + y * blending.y + z * blending.z;
    color = vec4( color.xyz, 1.0 );

    //color = vec4( norm, 1.0 );
    //color = vec4( geoCol, 1.0 );
}
