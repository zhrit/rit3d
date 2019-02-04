#pragma once
#include "RCommon.h"
#include "Mesh.h"
#include "IComponent.h"
#include "Material.h"

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

