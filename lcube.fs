#version 440

in vec3 cube_texel;
uniform samplerCube sampler;

in vec3 position_eye, normal_eye;

// supposed to be just the view matrix
uniform mat4 MV;

out vec4 finalColor;


vec3 light_position_world = vec3(24000.0, 9000.0, 24000.0);
vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.2, 0.2, 0.2);

vec3 Ks = vec3(0.5, 0.5, 0.5);
vec3 Kd = vec3(1.0, 0.5, 0.0);
vec3 Ka = vec3(0.5, 0.5, 0.5);

float spec_exp = 25.0;

void main() {

    // ambient intensity

    vec3 Ia = La * Ka;


    // diffuse intensity

    vec3 light_position_eye = vec3(MV * vec4(light_position_world, 1.0));
    vec3 distance_to_light_eye = light_position_eye - position_eye;
    vec3 direction_to_light_eye = normalize(distance_to_light_eye);
    float dot_prod = dot(direction_to_light_eye, normal_eye);
    dot_prod = max(dot_prod, 0.0);
        
    vec3 Id = Ld * Kd * dot_prod;
   
    
    
    
    // specular intensity

    vec3 reflection_eye = reflect(-direction_to_light_eye, normal_eye);
    vec3 surface_to_viewer_eye = normalize(-position_eye);
    float dot_prod_specular = dot(reflection_eye, surface_to_viewer_eye);
    dot_prod_specular = max(dot_prod_specular, 0.0);
    float specular_factor = pow(dot_prod_specular, spec_exp);
    
    vec3 Is = Ls * Ks * specular_factor;
    
       
    finalColor = texture(sampler, cube_texel) + vec4(Is+Id+Ia, 1.0);
    //finalColor = vec4(Is+Id+Ia, 1.0);
}