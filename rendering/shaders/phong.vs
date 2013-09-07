#version 330

in vec3 vVertex;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec2 UV;
smooth out vec3 vSmoothLightDir;

uniform mat4 mvp;
uniform mat4 mv;
uniform vec3 lightPosition;

void main()
{
    // Get vertex position in eye coordinates
    vec4 vPos4 = mv * vec4(vVertex, 1.0);
    vec3 vPos3 = vPos4.xyz / vPos4.w;

    // Get vector to light source
    vSmoothLightDir = normalize(lightPosition - vPos3);

    // Transform the geometry
    gl_Position = mvp * vec4(vVertex, 1.0f);
    UV = vUV;
}
