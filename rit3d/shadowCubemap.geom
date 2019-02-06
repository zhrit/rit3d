#version 330 core
layout (triangles) in;//以三角形图元输入
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 uShadowMatrices[6];

//每一次EmitVertex(),out的变量都会输出一次
out vec4 fragPos;// FragPos from GS (output per emitvertex)

void main() {
	for(int face = 0; face < 6; face++) {
		gl_Layer = face;
		for(int i = 0; i < 3; i++) {
			fragPos = gl_in[i].gl_Position;
			gl_Position = uShadowMatrices[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();//注意EndPrimitive()的位置不是在for外面
	}
}