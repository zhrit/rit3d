#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uSpirit;
uniform float uBrightness;

void main () {
	vec4 color = texture(uSpirit, TexCoord);
	//if(color.a < 0.1)
		//discard;
	vec3 result = color.xyz * uBrightness;
	//result = vec3(1.0f) - exp(-result);
	FragColor = vec4(result, color.a);
}