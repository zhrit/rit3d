#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 TexCoord; 

void main() {
    vec3 pos = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
	float zindex = pos.z * 0.5 + 0.5;
	float size = 2.0 * (1.0 - zindex);
    gl_Position = vec4(pos, 1.0) + vec4(-size, -size, 0.0, 0.0);
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();
    gl_Position = vec4(pos, 1.0) + vec4(size, -size, 0.0, 0.0);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();
    gl_Position = vec4(pos, 1.0) + vec4(-size, size, 0.0, 0.0);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();
	gl_Position = vec4(pos, 1.0) + vec4(size, size, 0.0, 0.0);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}