#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;


out vec2 vtxTexCoord;
out vec4 vtxNormalQuat;
out vec3 vtxView;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
    // Calculate position
    vec4 pos = modelMatrix * vec4(inPos, 1.0);
    vtxView = -pos.xyz;
    gl_Position = projectionMatrix * pos;

    // Calculate normalquat
    vec3 normal = normalize(mat3(modelMatrix) * inNormal);
    float z = (1.0 + normal.z) / 2.0;
    vtxNormalQuat = vec4(1.0, 0.0, 0.0, 0.0);
    if (z > 0.0)
    {
        vtxNormalQuat.z = sqrt(z);
        vtxNormalQuat.xy = normal.xy / (2.0 * vtxNormalQuat.z);
    }

    // Calculate texcoord
    vtxTexCoord = inTexCoord;
}