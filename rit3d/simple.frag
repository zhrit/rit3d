#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 color;
uniform int hasTex;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    //FragColor = vec4(color, 1.0f);
	FragColor = texture(texture0, TexCoord) * hasTex + vec4(color, 1.0f) * (1 - hasTex);
}