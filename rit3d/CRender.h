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
	std::vector<Mesh*> m_meshs;

	Material* m_mat;

	RBool m_castShadow{ false };

	RBool m_recieveShadow{ false };


public:
	static CRender* CreateInstance();

	void addMesh(Mesh* _mesh);

	void setMaterial(Material* _mat);

	RBool isCastShadow() const;
	void castShadow(RBool _cast);

	RBool isRecieveShadow() const;
	void recieveShadow(RBool _recieve);

};

