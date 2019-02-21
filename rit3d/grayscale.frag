#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float uExposure;
uniform sampler2D uTexture;
uniform sampler2D uBloomMap;
uniform float uBloomValue;

void main () {
	vec3 result = vec3(texture(uTexture, TexCoord));
	result = vec3(1.0f) - exp(-result * uExposure);
	float ave = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;//人眼对绿色更加敏感
	FragColor = vec4(ave, ave, ave, 1.0f);
}