#pragma once

#include "RCommon.h"

class Mesh {
public:
	static Mesh* Create(RString _name, RFloat* verArray, RInt verSize, RUInt* indArray, RInt indSize);
	static void Destroy(Mesh* _mesh);
private:
	Mesh(RString _name, RFloat* verArray, RInt verSize, RUInt* indArray, RInt indSize);
	~Mesh();

	RString m_name;

	//vertices∏Ò Ω£∫(p0,p1,p2,n0,n1,n2,c0,c1,c2,uv0,uv1)
	//              position,normal  ,color,   uv
	RFloat* m_vertices{ nullptr };
	RUInt* m_indices{ nullptr };

	RInt m_vertexCount{ 0 };
	RInt m_faceCount{ 0 };

	RUInt m_vertexArray;
	RUInt m_vertexBuffer;
	RUInt m_elementBuffer;

	void _setVertices(RFloat* pData, RInt size);
	void _setIndices(RUInt* pData, RInt size);
	void _createBuffer();

	void _destroy();

public:
	RInt getVertexCount() const;
	RInt getFaceCount() const;

	RUInt getVAO() const;

	void deleteBuffer();
};