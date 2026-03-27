#version 330 core

in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D Texture;

void main()
{
	fragColor = texture(Texture, TexCoords);
}
