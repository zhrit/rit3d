#include "pch.h"
#include "Mesh.h"

Mesh* Mesh::Create(RString _name, RFloat* verArray, RInt verSize, RUInt* indArray, RInt indSize) {
	return new Mesh(_name, verArray, verSize, indArray, indSize);
}
void Mesh::Destroy(Mesh* _mesh) {
	delete _mesh;
	_mesh = nullptr;
}

void Mesh::_destroy() {
	if (m_vertices != nullptr) {
		delete[] m_vertices;
		m_vertices = nullptr;
	}
	if (m_indices != nullptr) {
		delete[] m_indices;
		m_indices = nullptr;
	}
	deleteBuffer();
}

Mesh::Mesh(RString _name, RFloat* verArray, RInt verSize, RUInt* indArray, RInt indSize) {
	m_name = _name;
	if (verArray != nullptr && verSize > 0) {
		_setVertices(verArray, verSize);
	}
	if (indArray != nullptr && indArray > 0) {
		_setIndices(indArray, indSize);
	}
	_createBuffer();
}

Mesh::~Mesh() {
	_destroy();
}

void Mesh::_setVertices(RFloat* pData, RInt size) {
	if (m_vertices != nullptr) {
		delete[] m_vertices;
		m_vertices = nullptr;
	}
	m_vertices = new RFloat[size];
	for (int i = 0; i < size; i++) {
		m_vertices[i] = pData[i];
	}
	m_vertexCount = size / 11;
}
void Mesh::_setIndices(RUInt* pData, RInt size) {
	if (m_indices != nullptr) {
		delete[] m_indices;
		m_indices = nullptr;
	}
	m_indices = new RUInt[size];
	for (int i = 0; i < size; i++) {
		m_indices[i] = pData[i];
	}
	m_faceCount = size / 3;
}
RInt Mesh::getVertexCount() const {
	return m_vertexCount;
}
RInt Mesh::getFaceCount() const {
	return m_faceCount;
}

void Mesh::_createBuffer() {
	if (m_vertices == nullptr) {
		cout << "failed: no vertices data" << endl;
		return;
	}
	//´´½¨attay buffer
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_elementBuffer);

	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * 11 * sizeof(float), m_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faceCount * 3 * sizeof(unsigned int), m_indices, GL_STATIC_DRAW);
	//attribute
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//tangent
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//uv
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);
}

void Mesh::deleteBuffer() {
	glDeleteVertexArrays(1, &m_vertexArray);
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_elementBuffer);
}

RUInt Mesh::getVAO() const {
	return m_vertexArray;
}
