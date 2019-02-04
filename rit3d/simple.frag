out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 uColor;
uniform int uHasTex;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

void main()
{
    //FragColor = vec4(color, 1.0f);
	FragColor = texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex);
}