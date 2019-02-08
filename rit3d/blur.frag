#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uTexture;

const float offset = 1.0f / 300.0f;

void main () {
	vec2 offsets[9] = {
		vec2(-offset, offset), //左上
		vec2(0.0f, offset),     //正上
		vec2(offset, offset),  //右上
		vec2(-offset, 0.0f),   //左中
		vec2(0.0f, 0.0f),       //正中
		vec2(offset, 0.0f),    //右中
		vec2(-offset, -offset),//左下
		vec2(0.0f, -offset),    //正下
		vec2(offset, -offset), //右下
	};

	float kernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(uTexture, TexCoord + offsets[i]));
	}
	vec3 ave = vec3(0.0f);
	for(int i = 0; i < 9; i++) {
		ave += sampleTex[i] * kernel[i];
	}
	FragColor = vec4(ave, 1.0f);
}