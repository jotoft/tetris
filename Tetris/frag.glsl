#version 150

in vec2 Texcoord;
in vec3 Color;


out vec4 outColor;

uniform sampler2D tex;

void main()
{
	 outColor = texture( tex, Texcoord ) * vec4( Color, 1.0 ) * 1.5;
}