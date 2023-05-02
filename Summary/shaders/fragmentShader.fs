#version 330 core

out vec4 FragColor;

in vec3 outCol;
in vec2 outTex;

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, outTex);
}