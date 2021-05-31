#version 330

out vec4 FragColor;
in vec2 UV;
in float pressure;
in vec3 color;

void main()
{
	//float inside = 1.0-step(0.5, length(gl_PointCoord - vec2(0.5, 0.5)));
    //FragColor = vec4(inside, 0.0, 0.0, 0.5*inside);
	float dist = clamp(length(UV)*2.0, 0.0, 1.0);
    FragColor = vec4(color, (1.0-dist)*(1.0-dist));
}
