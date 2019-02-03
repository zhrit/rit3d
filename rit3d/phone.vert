layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;
layout(location = 2) in vec3 aCol;
layout(location = 3) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 nmodel;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	fragNormal = vec3(nmodel * vec4(aNor, 0.0f));//可能有问题
	fragPos = vec3(model * vec4(aPos, 1.0f));
	TexCoord = aTexCoord;
}