#version 330

in VertexData
{
    vec3 position;
    vec2 uv;
}vertex;

out vec2 uv;
out vec3 direction;

layout (std140) uniform TransformBlock
{
    mat4 mvp;
    mat4 mv;
    mat3 normalMatrix;
}transform;

layout (std140) uniform PointLight
{
    vec3 position;
    vec4 intensity;
}light;

void main()
{
    // Get vertex position in eye coordinates
    vec4 vPos4 = transform.mv * vec4(vertex.position, 1.0);
    vec3 vPos3 = vPos4.xyz / vPos4.w;

    // Get vector to light source
    direction = normalize(light.position - vPos3);

    // Transform the geometry
    gl_Position = transform.mvp * vec4(vertex.position, 1.0f);
    uv = vertex.uv;
}
