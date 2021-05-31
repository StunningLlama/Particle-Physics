#version 330

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in float pressureIn;
layout (location = 3) in vec3 colorIn;
out vec2 UV;
out float pressure;
out vec3 color;

uniform mat3 transform;

void main()
{
    gl_Position = vec4((transform*vec3(Position, 1.0)).xy, 0.0, 1.0);
	UV = vertexUV;
	pressure = pressureIn;
	color = colorIn;
	//color = vec3(pressure*0.2, 0.0, 1.0);
}