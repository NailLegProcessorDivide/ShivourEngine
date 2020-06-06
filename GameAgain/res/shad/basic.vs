#version 330 core

layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 normAttr;
layout(location = 2) in vec2 texAttr;

out vec2 uv;
out vec3 pos;
out vec3 normal;

out vec4 worldPos;
out vec4 worldNormal;

uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

void main() {
    pos = posAttr;
    uv = texAttr;
    normal = normAttr;
    worldPos = modelMatrix*vec4(posAttr, 1.0);
    worldNormal = normalize(modelMatrix*vec4(normAttr, 0.0));
    gl_Position = cameraMatrix*worldPos;
}