#version 440 core

layout( triangles ) in;
layout( line_strip, max_vertices = 2 ) out;

layout( binding = 3 ) uniform sampler2D normals;

uniform mat4 MVP;

in vec2 teUVg[];
out vec3 geoCol;


mat3 align = mat3(
    1.0, 0.0, 0.0,
    0.0, cos(1.572), -sin(1.572),
    0.0, sin(1.572), cos(1.572) );
   
void main() {
    
    vec3 norm = ( texture( normals, teUVg[0] ) ).xyz;
    norm = align * norm;

    geoCol = abs( norm );

    for( int i=0; i<3; i++ ) {

        vec4 v0 = gl_in[i].gl_Position;
        gl_Position = v0;
        EmitVertex();

        vec4 v1 = v0 + vec4( norm , 0.0 );
        gl_Position = v1;
        EmitVertex();

        EndPrimitive();
    }

}
