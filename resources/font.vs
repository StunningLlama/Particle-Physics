#version 330 core

layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat3 transform;

void main()
{
    gl_Position = vec4((transform*vec3(vertex.xy, 1.0)).xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}