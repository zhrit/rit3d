#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uTexture;
uniform bool uHorizontal;

const float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main () {
	vec2 offset = 1.0f / textureSize(uTexture, 0);
	vec3 result = texture(uTexture, TexCoord).rgb * weight[0];
	if (uHorizontal) {
		for(int i = 1; i < 5; i++) {
			result += texture(uTexture, TexCoord + vec2(offset.x * i, 0.0f)).rgb * weight[i];
			result += texture(uTexture, TexCoord - vec2(offset.x * i, 0.0f)).rgb * weight[i];
		}
	}
	else {
		for(int i = 1; i < 5; i++) {
			result += texture(uTexture, TexCoord + vec2(0.0f, offset.x * i)).rgb * weight[i];
			result += texture(uTexture, TexCoord - vec2(0.0f, offset.x * i)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0f);
}