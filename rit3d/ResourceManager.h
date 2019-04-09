#pragma once
#include "RCommon.h"

class Mesh;
class Material;
class Texture;
class GLProgram;
class RGameObject;
class RScene;
class CRender;

class ResourceManager
{
public:
	static ResourceManager *Instance();
private:
	ResourceManager();
	virtual ~ResourceManager();

	static ResourceManager *m_instance;

	//网格池
	std::map<RString, Mesh*> meshPool;
	//材质池
	std::map<RString, Material*> materialPool;
	//纹理池
	std::map<RString, Texture*> texturePool;
	//着色器池
	std::map<RString, GLProgram*> shaderPool;

	//根据vertices和indices计算normals
	void _calcNormal(RFloat* vs, RInt vSize, RUInt* id, RInt iSize, RFloat* nm);
	//根据vertices和indices计算tangents
	void _calcTangent(RFloat* vs, RInt vSize, RUInt* id, RInt iSize, RFloat* uv, RFloat* tg);
	//加载模型时处理节点
	void _processNode(aiNode* node, const aiScene* scene, CRender* render, RString modelDir);
	//加载模型时处理网格
	void _processMesh(aiMesh* mesh, const aiScene* scene, CRender* render, RString modelDir);
	//加载模型得材质纹理
	void _loadMaterialTexture(Material* mat, aiMaterial* aiMat, RString modelDir);
public:
	//创建新网格
	Mesh* createMesh(const RString& _name, RFloat* verArray, RInt verSize, RUInt* indArray, RInt indSize);
	//根据名称移除网格资源
	void deleteMesh(const RString& _name);
	//根据名称获取网格资源
	Mesh* getMesh(const RString& _name);

	//创建新材质
	Material* createMaterial(const RString& _name);
	//根据名称移除材质资源
	void deleteMaterial(const RString& _name);
	//根据名称获取材质资源
	Material* getMaterial(const RString& _name);

	//创建新纹理
	Texture* createTexture(const RString& imagePath);
	//根据名称移除纹理资源
	void deleteTexture(const RString& _name);
	//根据名称获取纹理资源
	Texture* getTexture(const RString& _name);

	//创建新着色器
	GLProgram* createShader(const RString& _name, const RString& vertexPath, const RString& fragmentPath, const RString& defines, const RString& geometryPath = "");
	//根据名称移除着色器资源
	void deleteShader(const RString& _name);
	//根据名称获取着色器资源
	GLProgram* getShader(const RString& _name);

	//创建默认网格资源
	void createDefaultMesh();
	//创建默认材质资源
	void createDefaultMaterial();
	//创建默认shader
	void createDefaultShader();

	//清空所有网格资源
	void clearMesh();
	//清空所有材质资源
	void clearMaterial();
	//清空所有纹理资源
	void clearTexture();
	//清空所有shader
	void clearShader();
	//清空所有资源
	void clearAll();

	//加载模型
	RGameObject* loadModel(RString path, RScene* sce);
};

