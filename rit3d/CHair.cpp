#include "pch.h"
#include "CHair.h"

CHair::CHair() {
	m_type = COMPTYPE::HAIR;
	_init();
}


CHair::~CHair() {

}

CHair* CHair::CreateInstance() {
	return new CHair();
};


void CHair::_init() {
	//在这里创建一个圆形的头皮出来
	//sphere
	const RUInt X_SEGMENTS = 20;
	const RUInt Y_SEGMENTS = 4;
	const RUInt N = X_SEGMENTS * Y_SEGMENTS;
	const RUInt vc = (X_SEGMENTS * Y_SEGMENTS) * 6;
	const RUInt ic = X_SEGMENTS * (Y_SEGMENTS - 1) * 6;
	RFloat vertices_sphere[vc] = { 0.0f };
	RUInt i = 0;
	for (RUInt y = 0; y < Y_SEGMENTS; y++) {
		for (RUInt x = 0; x < X_SEGMENTS; x++) {
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = 0.5f * sin(xSegment * PI2) * cos(ySegment * PI_DIV_2);
			float yPos = 0.5f * sin(ySegment * PI_DIV_2);
			float zPos = 0.5f * cos(xSegment * PI2) * cos(ySegment * PI_DIV_2);
			vertices_sphere[i * 6] = xPos;
			vertices_sphere[i * 6 + 1] = yPos;
			vertices_sphere[i * 6 + 2] = zPos;
			vertices_sphere[i * 6 + 3] = 2.0f * xPos;
			vertices_sphere[i * 6 + 4] = 2.0f * yPos;
			vertices_sphere[i * 6 + 5] = 2.0f * zPos;
			i++;
		}
	}
	RUInt indices_sphere[ic] = { 0 };
	i = 0;
	for (RUInt y = 0; y < Y_SEGMENTS - 1; y++) {
		for (RUInt x = 0; x < X_SEGMENTS; x++) {
			indices_sphere[i++] = y * X_SEGMENTS + x;
			indices_sphere[i++] = y * X_SEGMENTS + ((x + 1) % X_SEGMENTS);
			indices_sphere[i++] = (y + 1) * X_SEGMENTS + x;
			indices_sphere[i++] = (y + 1) * X_SEGMENTS + x;
			indices_sphere[i++] = y * X_SEGMENTS + ((x + 1) % X_SEGMENTS);
			indices_sphere[i++] = (y + 1) * X_SEGMENTS + ((x + 1) % X_SEGMENTS);
		}
	}
	//根据头皮数据生成顶点数据
	for (RUInt i = 0; i < N; i++) {
		float dl = 0.0f;
		float xp = vertices_sphere[6 * i];
		float yp = vertices_sphere[6 * i + 1];
		float zp = vertices_sphere[6 * i + 2];
		float xn = vertices_sphere[6 * i + 3];
		float yn = vertices_sphere[6 * i + 4];
		float zn = vertices_sphere[6 * i + 5];
		glm::vec3 pos = glm::vec3(xp, yp, zp);
		glm::vec3 nor = glm::vec3(xn, yn, zn);
		for (int j = 0; j < nodeInStrand; j++) {
			HairNode node;
			node.p0 = node.p1 = pos + dl * nor;
			node.maxLength = dl;
			m_drawData.push_back(node.p1);
			m_nodes.push_back(node);

			if (dl < 1e-6f) {
				dl = minLength;
			}
			else {
				dl *= 2.0f;
				dl = (dl <= 0.4f) ? dl : 0.4f;
			}
			pos = node.p1;
		}
	}
	//一条发束7个节点
	for (int i = 0; i < N; i++) {
		HairStrand strand;
		strand.nodeStart = nodeInStrand * i;
		strand.nodeEnd = nodeInStrand * (i + 1) - 1;
		strand.rootP = m_nodes[strand.nodeStart].p1;
		m_strands.push_back(strand);
	}

	//创建attay buffer
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_drawData.size() * sizeof(float), &m_drawData[0], GL_STREAM_DRAW);
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}