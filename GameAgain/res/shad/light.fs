#version 330 core

out vec3 col;

in vec2 uv;
in vec4 worldNormal;
in vec4 worldPos;

uniform sampler2D myTextureSampler;

uniform vec3 camPos;

uniform vec3 lightPos;
uniform vec3 lightCol;

void main() {

    vec3 lightLine = lightPos - worldPos.xyz;
    vec3 lightDir = normalize(lightLine);
    vec3 camDir = normalize(camPos - worldPos.xyz);

    float lightAngle = min(max(dot(lightDir, worldNormal.xyz), 0), 1);
    float camAngle = min(max(dot(lightDir, worldNormal.xyz), 0), 1);

    vec3 baseCol = texture( myTextureSampler, uv ).rgb;

    col = baseCol * lightAngle;
}