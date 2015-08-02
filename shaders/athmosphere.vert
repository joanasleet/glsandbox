#version 330

layout( location = 0 ) in vec3 pos;

uniform mat4 MVP;
uniform vec3 camPos;
uniform float gTime;

/* scattering constants */
const float f1 = 0.650;
const float f2 = 0.570;
const float f3 = 0.475;

/* some other stuff */
vec3 v3InvWavelength = vec3(1.0 / pow( f1, 4.0), 1.0 / pow( f2, 4.0), 1.0 / pow( f3, 4.0));
float fCameraHeight = length(camPos);
float fInnerRadius = 10.0;
float fOuterRadius = 10.25;
float radius = fOuterRadius;
float fESun = 10.0;
float fKr = 0.0025;
float fKm = 0.0010;
float fPI = 3.141592654;
float fKrESun = fKr * fESun;
float fKmESun = fKm * fESun;
float fKr4PI = fKr * 4.0 * fPI;
float fKm4PI = fKm * 4.0 * fPI;
float fScale = 1.0 / (fOuterRadius - fInnerRadius);
float fScaleDepth = 0.25;
float fScaleOverScaleDepth = fScale / fScaleDepth;

const int nSamples = 20;
const float fSamples = float( nSamples );

vec3 sunPosition(float time) {

    const float timeScaleSun = 0.02;
    vec3 sunPos = vec3(0.0, 0.0, -radius);
    float x = radius * cos(timeScaleSun * time) * cos(timeScaleSun * time );
    float y = radius * cos(timeScaleSun * time) * sin(timeScaleSun * time );
    float z = radius * sin(timeScaleSun * time);
    sunPos = vec3(x, z, -y);
    return sunPos;
}

out vec4 vsColor;
out vec4 vsSecColor;
out vec3 vsLightPos;
out vec3 vsDirection;

float scale(float angle) {
    float x = 1.0 - angle;
    return (fScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.8 + x * 5.25)))));
}

void athmosScatter(vec3 pos) {

    vec3 v3Pos = pos;
    vec3 v3Ray = v3Pos - camPos;
    float fFar = length(v3Ray);
    v3Ray /= fFar;

    vec3 v3Start = camPos;
    float fHeight = length(v3Start);
    float fDepth = exp(-fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
    float fStartAngle = dot(v3Ray, v3Start) / fHeight;
    float fStartOffset = fDepth * scale(fStartAngle);

    float fSampleLength = fFar / fSamples;
    float fScaledLength = fSampleLength * fScale;
    vec3 v3SampleRay = v3Ray * fSampleLength;
    vec3 v3SamplePoint = v3Start + 0.5 * v3SampleRay;

    vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
    vec3 lightPos = normalize(sunPosition(gTime));

    for (int i = 0; i < nSamples; i++) {

        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
        float fLightAngle = dot(lightPos, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;

        float fScatter = (fStartOffset + fDepth * (scale(fLightAngle) - scale(fCameraAngle)));
        vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }

    vsSecColor.rgb = v3FrontColor * fKmESun;
    vsColor.rgb = v3FrontColor * (v3InvWavelength * fKrESun);
    vsDirection = camPos - v3Pos;
}

void main() {

    /* set position */
    gl_Position = MVP * pos;

    /* calc sky color */
    athmosScatter(pos);
}
