#version 330

in vec3 position;
in vec2 uv;

out vec2 UV;
out vec3 direction;

layout (std140) uniform TransformBlock
{
    mat4 mvp;
    mat4 mv;
    mat3 normal;
}transform;

layout (std140) uniform PointLight
{
    vec3 position;
    vec4 intensity;
}light;

void main()
{
    // Get vertex position in eye coordinates
    vec4 vPos4 = transform.mv * vec4(position, 1.0);
    vec3 vPos3 = vPos4.xyz / vPos4.w;

    // Get vector to light source
    direction = normalize(light.position - vPos3);

    // Transform the geometry
    gl_Position = transform.mvp * vec4(position, 1.0f);
    UV = uv;
}
