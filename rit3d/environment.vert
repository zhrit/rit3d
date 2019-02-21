layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;

out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 nmodel;

void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0f));
	fragNormal = vec3(nmodel * vec4(aNor, 0.0f));;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}