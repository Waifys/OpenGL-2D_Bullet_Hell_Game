#version 330 core

in vec2 TexCoord;

out vec4 fragColor;
in float depth;

uniform sampler2D ourTexture;
uniform float time; 
 
void main()
{

    vec2 scrollingCoord = TexCoord + vec2(0.0, time * 0.1);
    vec4 texColor = texture(ourTexture, scrollingCoord);

    float fogMin = 5.0; 
    float fogMax = 20.0; 
    vec4 fogColor = vec4(0.05, 0.02, 0.1, 1.0); 

    float fogFactor = clamp((depth*2.3 - fogMin) / (fogMax - fogMin), 0.0, 1.0);

    fragColor = mix(texColor, fogColor, fogFactor);
}
