#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aTan;

out vec3 tangent;
out vec3 position;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	tangent = normalize(aTan);
	//tangent = vec3(0.0f, 0.0f, 0.0f);
	gl_Position = uProjection * uView * vec4(aPos, 1.0f);
}