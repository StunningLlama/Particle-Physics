#version 330

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 vertexUV;

out vec2 UV;

void main()
{
    gl_Position = vec4(Position, 0.0, 1.0);
	UV = vertexUV;
}