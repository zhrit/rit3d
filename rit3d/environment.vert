layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;

out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNModel;

void main()
{
	fragPos = vec3(uModel * vec4(aPos, 1.0f));
	fragNormal = vec3(uNModel * vec4(aNor, 0.0f));;
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
}