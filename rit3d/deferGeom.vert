#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aCol;
layout(location = 4) in vec2 aTexCoord;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 TexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNModel;

void main() {
	vec4 worldPos = uModel * vec4(aPos, 1.0f);
	fragPos = vec3(worldPos);
	gl_Position = uProjection * uView * worldPos;
	
	TexCoord = aTexCoord;
	fragNormal =  vec3(uNModel * vec4(aNor, 0.0f));
}