#version 330

out vec4 FragColor;
in vec2 UV;

uniform sampler2D BGtexture;

void main()
{
    FragColor = vec4(texture2D(BGtexture, UV).r*2.0-1.0,texture2D(BGtexture, UV).r*2.0-0.5, texture2D(BGtexture, UV).r*2.0, 1.0);
}
