#version 440

in vec2 texcoord;

out vec4 finalColor;

layout ( binding = 0 ) uniform sampler2D s1;
layout ( binding = 1 ) uniform sampler2D s2;
layout ( binding = 2 ) uniform sampler2D s3;

void main() {

    vec4 cliff = texture( s1, texcoord*200 );
    vec4 grass = texture( s2, texcoord*200 );
    vec4 blend = texture( s3, texcoord );
    float blendf = ( blend.r + blend.g + blend.b ) / 3.0f;

    vec4 c = blendf * grass + ( 1.0f - blendf ) * cliff;

    finalColor = vec4( c.rgb, 1.0f );
}
