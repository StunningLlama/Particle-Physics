#version 330

layout (location = 0) in vec2 Position;
layout (location = 1) in vec3 colorIn;
out vec3 color;

uniform mat3 transform;

void main()
{
    gl_Position = vec4((transform*vec3(Position, 1.0)).xy, 0.0, 1.0);
	color = colorIn;
}