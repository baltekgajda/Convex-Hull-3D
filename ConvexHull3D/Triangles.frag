#version 330 core

uniform vec4 color;

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = color;
}
