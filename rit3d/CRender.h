#pragma once
#include "RCommon.h"
#include "IComponent.h"

class Mesh;
class Material;
class CRender : public IComponent{
private:
	CRender();
	virtual ~CRender();

public:
	Mesh* m_mesh;

	Material* m_mat;

	RBool m_castShadow{ false };

	RBool m_recieveShadow{ false };


public:
	static CRender* CreateInstance();

	void setMesh(Mesh* _mesh);

	void setMaterial(Material* _mat);

	RBool isCastShadow() const;
	void castShadow(RBool _cast);

	RBool isRecieveShadow() const;
	void recieveShadow(RBool _recieve);

};

