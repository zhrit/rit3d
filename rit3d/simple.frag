layout (location = 0) out vec4 FragColor;
#ifdef BLOOM
layout (location = 1) out vec4 BrightColor;
#endif

in vec2 TexCoord;

uniform vec3 uColor;
uniform int uHasTex;
#ifdef BLOOM
uniform float uBloomValue;
#endif
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

void main()
{
    //FragColor = vec4(color, 1.0f);
	FragColor = texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex);
	#ifdef BLOOM
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	if(brightness > uBloomValue) {
		BrightColor = vec4(FragColor.rgb, 1.0f);
	}
	#endif
}