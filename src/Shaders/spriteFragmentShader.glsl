#version 330 core

in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D Texture;
uniform int amountOfSprites;
uniform int currentSprite;

void main()
{
	if(amountOfSprites > 1)
	{
		float spriteWidth = 1.0 / float(amountOfSprites);
        vec2 temp = vec2(TexCoords.x * spriteWidth + float(currentSprite) * spriteWidth, TexCoords.y);
        fragColor = texture(Texture, temp);
	}
	else
		fragColor = texture(Texture, TexCoords);
}