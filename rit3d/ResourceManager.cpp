#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "GLProgram.h"
#include "CRender.h"
#include "RGameObject.h"

ResourceManager *ResourceManager::Instance() {
	if (m_instance == nullptr) {
		m_instance = new ResourceManager();
	}
	return m_instance;
}

ResourceManager::ResourceManager() {
	meshPool.clear();
	materialPool.clear();
}
ResourceManager::~ResourceManager() {

}

ResourceManager *ResourceManager::m_instance = nullptr;

//创建新网格
Mesh* ResourceManager::createMesh(const RString& _name, RFloat* verArray, RInt verSize, RUInt* indArray, RInt indSize) {
	Mesh* mesh = Mesh::Create(_name, verArray, verSize, indArray, indSize);
	std::pair<std::map<RString, Mesh*>::iterator, bool> Insert_Pair;

	Insert_Pair = meshPool.insert(std::pair<RString, Mesh*>(_name, mesh));

	if (Insert_Pair.second == false) {
		Mesh::Destroy(mesh);
		cout << "已有同名网格，创建失败" << endl;
		return nullptr;
	}

	return mesh;
}

//根据名称移除网格资源
void ResourceManager::deleteMesh(const RString& _name) {
	std::map<RString, Mesh*>::iterator iter;
	iter = meshPool.find(_name);
	if (iter != meshPool.end()) {
		meshPool.erase(iter);
	}
}
//根据名称获取网格资源
Mesh* ResourceManager::getMesh(const RString& _name) {
	std::map<RString, Mesh*>::iterator iter;
	iter = meshPool.find(_name);
	if (iter != meshPool.end()) {
		return iter->second;
	}
	else {
		cout << "网格资源不存在！" << endl;
		return nullptr;
	}
}

//创建新材质
Material* ResourceManager::createMaterial(const RString& _name) {
	Material* mat = getMaterial(_name);
	if (!mat) {
		mat = Material::Create(_name);
		std::pair<std::map<RString, Material*>::iterator, bool> Insert_Pair;

		Insert_Pair = materialPool.insert(std::pair<RString, Material*>(_name, mat));

		if (Insert_Pair.second == false) {
			Material::Destroy(mat);
			cout << "已有同名材质，创建失败" << endl;
			return nullptr;
		}
	}

	return mat;
}

//根据名称移除材质资源
void ResourceManager::deleteMaterial(const RString& _name) {
	std::map<RString, Material*>::iterator iter;
	iter = materialPool.find(_name);
	if (iter != materialPool.end()) {
		materialPool.erase(iter);
	}
}
//根据名称获取材质资源
Material* ResourceManager::getMaterial(const RString& _name) {
	std::map<RString, Material*>::iterator iter;
	iter = materialPool.find(_name);
	if (iter != materialPool.end()) {
		return iter->second;
	}
	else {
		return nullptr;
	}
}

//创建新纹理
Texture* ResourceManager::createTexture(const RString& imagePath) {
	Texture* tex = getTexture(imagePath);
	if (!tex) {
		tex = Texture::Create(imagePath);
		std::pair<std::map<RString, Texture*>::iterator, bool> Insert_Pair;

		Insert_Pair = texturePool.insert(std::pair<RString, Texture*>(imagePath, tex));

		if (Insert_Pair.second == false) {
			Texture::Destroy(tex);
			cout << "已有同名纹理，创建失败" << endl;
			return nullptr;
		}
	}

	return tex;
}
//根据名称移除纹理资源
void ResourceManager::deleteTexture(const RString& _name) {
	std::map<RString, Texture*>::iterator iter;
	iter = texturePool.find(_name);
	if (iter != texturePool.end()) {
		texturePool.erase(iter);
	}
}
//根据名称获取纹理资源
Texture* ResourceManager::getTexture(const RString& _name) {
	std::map<RString, Texture*>::iterator iter;
	iter = texturePool.find(_name);
	if (iter != texturePool.end()) {
		return iter->second;
	}
	else {
		return nullptr;
	}
}

//创建新着色器
GLProgram* ResourceManager::createShader(const RString& _name, const RString& vertexPath, const RString& fragmentPath, const RString& defines, const RString& geometryPath) {
	GLProgram* shader = GLProgram::Create(_name, vertexPath, fragmentPath, defines, geometryPath);
	std::pair<std::map<RString, GLProgram*>::iterator, bool> Insert_Pair;
	Insert_Pair = shaderPool.insert(std::pair<RString, GLProgram*>(_name, shader));
	if (Insert_Pair.second == false) {
		GLProgram::Destroy(shader);
		shader = nullptr;
		cout << "已有同名shader，创建失败" << endl;
		return nullptr;
	}
	return shader;
}
//根据名称移除着色器资源
void ResourceManager::deleteShader(const RString& _name) {
	std::map<RString, GLProgram*>::iterator iter;
	iter = shaderPool.find(_name);
	if (iter != shaderPool.end()) {
		shaderPool.erase(iter);
	}
}
//根据名称获取着色器资源
GLProgram* ResourceManager::getShader(const RString& _name) {
	std::map<RString, GLProgram*>::iterator iter;
	iter = shaderPool.find(_name);
	if (iter != shaderPool.end()) {
		return iter->second;
	}
	else {
		cout << "shader不存在！" << endl;
		return nullptr;
	}
}

//创建默认网格资源
void ResourceManager::createDefaultMesh() {
	{
		//quad
		const RUInt vc = 4 * 14;
		const RUInt ic = 6;
		RFloat vertices_quad[] = {
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		};
		RUInt indices_quad[] = {
			0, 2, 1, 0, 3, 2
		};
		createMesh("quad", vertices_quad, vc, indices_quad, ic);
	}
	{
		//plane
		const RUInt vc = 121 * 14;
		const RUInt ic = 600;
		RFloat vertices_plane[vc];//121*14
		int k = 0;
		for (int i = -5; i <= 5; i++) {
			for (int j = -5; j <= 5; j++) {
				vertices_plane[k * 14] = (RFloat)j;
				vertices_plane[k * 14 + 1] = 0.0f;
				vertices_plane[k * 14 + 2] = (RFloat)i;
				vertices_plane[k * 14 + 3] = 0.0f;
				vertices_plane[k * 14 + 4] = 1.0f;
				vertices_plane[k * 14 + 5] = 0.0f;
				vertices_plane[k * 14 + 6] = 1.0f;
				vertices_plane[k * 14 + 7] = 0.0f;
				vertices_plane[k * 14 + 8] = 0.0f;
				vertices_plane[k * 14 + 9] = 1.0f;
				vertices_plane[k * 14 + 10] = 1.0f;
				vertices_plane[k * 14 + 11] = 1.0f;
				vertices_plane[k * 14 + 12] = ((RFloat)j + 5.0f)*0.1f;
				vertices_plane[k * 14 + 13] = (5.0f - (RFloat)i) * 0.1f;
				k++;
			}
		}
		k = 0;
		RUInt indices_plane[ic];
		for (int i = 0; i < 109; i++) {
			if (i % 11 == 10) {
				continue;
			}
			indices_plane[k * 6] = i;
			indices_plane[k * 6 + 1] = i + 12;
			indices_plane[k * 6 + 2] = i + 1;
			indices_plane[k * 6 + 3] = i;
			indices_plane[k * 6 + 4] = i + 11;
			indices_plane[k * 6 + 5] = i + 12;
			k++;
		}
		createMesh("plane", vertices_plane, vc, indices_plane, ic);
	}
	{
		//cube
		const RUInt vc = 24 * 14;
		const RUInt ic = 36;
		float vertices_cube[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f
		};
		RUInt indices_cube[] = {
			0,2,1,0,3,2,4,6,5,4,7,6,8,10,9,8,11,10,12,14,13,12,15,14,16,18,17,16,19,18,20,22,21,20,23,22
		};
		createMesh("cube", vertices_cube, vc, indices_cube, ic);
	}
	{
		//sphere
		const RUInt X_SEGMENTS = 64;
		const RUInt Y_SEGMENTS = 64;
		const RUInt vc = (X_SEGMENTS + 1) * (Y_SEGMENTS + 1) * 14;
		const RUInt ic = X_SEGMENTS * Y_SEGMENTS * 6;
		//4225个点,46475
		RFloat vertices_sphere[vc] = { 0.0f };
		RUInt i = 0;
		for (RUInt y = 0; y <= Y_SEGMENTS; y++) {
			for (RUInt x = 0; x <= X_SEGMENTS; x++) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = 0.5f * cos(xSegment * PI2) * sin(ySegment * PI);
				float yPos = 0.5f * cos(ySegment * PI);
				float zPos = 0.5f * sin(xSegment * PI2) * sin(ySegment * PI);
				vertices_sphere[i * 14] = xPos;
				vertices_sphere[i * 14 + 1] = yPos;
				vertices_sphere[i * 14 + 2] = zPos;
				vertices_sphere[i * 14 + 3] = 2.0f * xPos;
				vertices_sphere[i * 14 + 4] = 2.0f * yPos;
				vertices_sphere[i * 14 + 5] = 2.0f * zPos;

				float len = sqrt(xPos * xPos + zPos * zPos);
				vertices_sphere[i * 14 + 6] = -zPos / len;
				vertices_sphere[i * 14 + 7] = 0.0f;
				vertices_sphere[i * 14 + 8] = xPos / len;

				vertices_sphere[i * 14 + 9] = 1.0f;
				vertices_sphere[i * 14 + 10] = 1.0f;
				vertices_sphere[i * 14 + 11] = 1.0f;
				vertices_sphere[i * 14 + 12] = xSegment;
				vertices_sphere[i * 14 + 13] = ySegment;
				i++;
			}
		}
		RUInt indices_sphere[ic] = { 0 };
		i = 0;
		for (RUInt y = 0; y < Y_SEGMENTS; y++) {
			for (RUInt x = 0; x < X_SEGMENTS; x++) {
				indices_sphere[i++] = y * (X_SEGMENTS + 1) + x;
				indices_sphere[i++] = (y + 1) * (X_SEGMENTS + 1) + x;
				indices_sphere[i++] = (y + 1) * (X_SEGMENTS + 1) + 1 + x;
				indices_sphere[i++] = y * (X_SEGMENTS + 1) + x;
				indices_sphere[i++] = (y + 1) * (X_SEGMENTS + 1) + 1 + x;
				indices_sphere[i++] = y * (X_SEGMENTS + 1) + 1 + x;
			}
		}
		createMesh("sphere", vertices_sphere, vc, indices_sphere, ic);
	}
	{
		//cone 60份 122个点
		const RUInt vc = 122 * 14;
		const RUInt ic = 360;
		RFloat vertices[366] = { 0.0f };
		int k = 0;
		for (int i = 0; i < 60; i++) {
			vertices[k++] = 0.5f * cos(PI2 / 60 * i);
			vertices[k++] = 0.0f;
			vertices[k++] = 0.5f * sin(PI2 / 60 * i);
			vertices[k++] = 0.5f * cos(PI2 / 60 * i);
			vertices[k++] = 0.0f;
			vertices[k++] = 0.5f * sin(PI2 / 60 * i);
		}
		vertices[k++] = 0.0f;
		vertices[k++] = 0.0f;
		vertices[k++] = 0.0f;
		vertices[k++] = 0.0f;
		vertices[k++] = 1.0f;
		vertices[k++] = 0.0f;

		RUInt indices_cone[360];
		k = 0;
		for (int i = 0; i < 60; i++) {
			indices_cone[k++] = 121;
			indices_cone[k++] = i * 2;
			if (i == 59) {
				indices_cone[k++] = 0;
			}
			else {
				indices_cone[k++] = 2 + i * 2;
			}
			indices_cone[k++] = 120;
			if (i == 59) {
				indices_cone[k++] = 1;
			}
			else {
				indices_cone[k++] = 3 + i * 2;
			}
			indices_cone[k++] = 1 + i * 2;
		}

		RFloat normals[366] = { 0.0f };
		_calcNormal(vertices, 366, indices_cone, 360, normals);

		RFloat uv[244];
		k = 0;
		for (int i = 0; i < 60; i++) {
			uv[k++] = 0.5f + 0.5f * sin(PI2 / 60 * i);
			uv[k++] = 0.5f + 0.5f * cos(PI2 / 60 * i);
			uv[k++] = 0.5f + 0.5f * sin(PI2 / 60 * i);
			uv[k++] = 0.5f + 0.5f * cos(PI2 / 60 * i);
		}
		uv[k++] = 0.5, uv[k++] = 0.5, uv[k++] = 0.5, uv[k++] = 0.5;

		RFloat tangents[366] = { 0.0f };
		_calcTangent(vertices, 366, indices_cone, 360, uv, tangents);

		RFloat vertices_cone[vc];
		for (int i = 0; i < 122; i++) {
			vertices_cone[i * 14] = vertices[i * 3];
			vertices_cone[i * 14 + 1] = vertices[i * 3 + 1];
			vertices_cone[i * 14 + 2] = vertices[i * 3 + 2];
			vertices_cone[i * 14 + 3] = normals[i * 3];
			vertices_cone[i * 14 + 4] = normals[i * 3 + 1];
			vertices_cone[i * 14 + 5] = normals[i * 3 + 2];
			vertices_cone[i * 14 + 6] = tangents[i * 3];
			vertices_cone[i * 14 + 4] = tangents[i * 3 + 1];
			vertices_cone[i * 14 + 5] = tangents[i * 3 + 2];
			vertices_cone[i * 14 + 9] = 1.0f;
			vertices_cone[i * 14 + 10] = 1.0f;
			vertices_cone[i * 14 + 11] = 1.0f;
			vertices_cone[i * 14 + 12] = uv[i * 2];
			vertices_cone[i * 14 + 13] = uv[i * 2] + 1;
		}
		createMesh("cone", vertices_cone, vc, indices_cone, ic);
	}
	{
		//cylinder // 60份 244个点
		const RUInt vc = 244 * 14;
		const RUInt ic = 720;
		RFloat vertices[732] = { 0.0f };
		int k = 0;
		for (int i = 0; i <= 60; i++) {
			vertices[3 * k] = 0.5f * cos(PI2 / 60 * i);
			vertices[3 * k + 1] = 0.5f;
			vertices[3 * k + 2] = 0.5f * sin(PI2 / 60 * i);
			k++;
			vertices[3 * k] = 0.5f * cos(PI2 / 60 * i);
			vertices[3 * k + 1] = -0.5f;
			vertices[3 * k + 2] = 0.5f * sin(PI2 / 60 * i);
			k++;
		}
		for (int i = 0; i < 60; i++) {
			vertices[3 * k] = 0.5f * cos(PI2 / 60 * i);
			vertices[3 * k + 1] = 0.5f;
			vertices[3 * k + 2] = 0.5f * sin(PI2 / 60 * i);
			k++;
			vertices[3 * k] = 0.5f * cos(PI2 / 60 * i);
			vertices[3 * k + 1] = -0.5f;
			vertices[3 * k + 2] = 0.5f * sin(PI2 / 60 * i);
			k++;
		}
		vertices[3 * k] = 0.0f;
		vertices[3 * k + 1] = 0.5f;
		vertices[3 * k + 2] = 0.0f;
		k++;
		vertices[3 * k] = 0.0f;
		vertices[3 * k + 1] = -0.5f;
		vertices[3 * k + 2] = 0.0f;

		RUInt indices_cylinder[ic];
		k = 0;
		for (int i = 0; i < 60; i++) {
			indices_cylinder[k++] = i * 2;
			indices_cylinder[k++] = i * 2 + 1;
			indices_cylinder[k++] = i * 2 + 2;
			indices_cylinder[k++] = i * 2 + 1;
			indices_cylinder[k++] = i * 2 + 3;
			indices_cylinder[k++] = i * 2 + 2;
		}
		for (int i = 0; i < 60; i++) {
			indices_cylinder[k++] = 242;
			indices_cylinder[k++] = 122 + i * 2;
			if (i == 59) {
				indices_cylinder[k++] = 122;
			}
			else {
				indices_cylinder[k++] = 124 + i * 2;
			}
			indices_cylinder[k++] = 243;
			if (i == 59) {
				indices_cylinder[k++] = 123;
			}
			else {
				indices_cylinder[k++] = 125 + i * 2;
			}
			indices_cylinder[k++] = 123 + i * 2;
		}
		RFloat normals[732] = { 0.0f };
		_calcNormal(vertices, 732, indices_cylinder, 720, normals);

		RFloat uv[488];
		k = 0;
		for (int i = 0; i <= 60; i++) {
			uv[k++] = 1.0f / 60.0f * i;
			uv[k++] = 1.0f;
			uv[k++] = 1.0f / 60.0f * i;
			uv[k++] = 0.0f;
		}
		for (int i = 0; i < 60; i++) {
			uv[k++] = 0.5f + 0.5f * sin(PI2 / 60 * i);
			uv[k++] = 0.5f + 0.5f * cos(PI2 / 60 * i);
			uv[k++] = 0.5f + 0.5f * sin(PI2 / 60 * i);
			uv[k++] = 0.5f + 0.5f * cos(PI2 / 60 * i);
		}
		uv[k++] = 0.5, uv[k++] = 0.5, uv[k++] = 0.5, uv[k++] = 0.5;

		RFloat tangents[732] = { 0.0f };
		_calcTangent(vertices, 732, indices_cylinder, 720, uv, tangents);

		RFloat vertices_cylinder[vc];
		for (int i = 0; i < 244; i++) {
			vertices_cylinder[i * 14] = vertices[i * 3];
			vertices_cylinder[i * 14 + 1] = vertices[i * 3 + 1];
			vertices_cylinder[i * 14 + 2] = vertices[i * 3 + 2];
			vertices_cylinder[i * 14 + 3] = normals[i * 3];
			vertices_cylinder[i * 14 + 4] = normals[i * 3 + 1];
			vertices_cylinder[i * 14 + 5] = normals[i * 3 + 2];
			vertices_cylinder[i * 14 + 6] = tangents[i * 3];
			vertices_cylinder[i * 14 + 4] = tangents[i * 3 + 1];
			vertices_cylinder[i * 14 + 5] = tangents[i * 3 + 2];
			vertices_cylinder[i * 14 + 9] = 1.0f;
			vertices_cylinder[i * 14 + 10] = 1.0f;
			vertices_cylinder[i * 14 + 11] = 1.0f;
			vertices_cylinder[i * 14 + 12] = uv[i * 2];
			vertices_cylinder[i * 14 + 13] = uv[i * 2] + 1;
		}
		createMesh("cylinder", vertices_cylinder, vc, indices_cylinder, ic);
	}
}

//创建默认材质资源
void ResourceManager::createDefaultMaterial() {

}

//创建默认shader
void ResourceManager::createDefaultShader() {
	//createShader("simple", "simple.vert", "simple.frag");
	createShader("shadowMap", "shadowMap.vert", "shadowMap.frag", "", "");
	createShader("shadowCubemap", "shadowCubemap.vert", "shadowCubemap.frag", "", "shadowCubemap.geom");
	createShader("postProcess", "postProcess.vert", "postProcess.frag", "", "");
	createShader("skybox", "skybox.vert", "skybox.frag", "", "");
	createShader("gaussianBlur", "postProcess.vert", "gaussianBlur.frag", "", "");
	createShader("deferGeom", "deferGeom.vert", "deferGeom.frag", "", "");
	createShader("deferLight", "deferLight.vert", "deferLight.frag", "", "");
}

//根据vertices和indices计算normals
void ResourceManager::_calcNormal(RFloat* vs, RInt vSize, RUInt* id, RInt iSize, RFloat* nm) {
	for (int i = 0; i < vSize; i++) {
		nm[i] = 0.0f;
	}
	for (int i = 0; i < iSize; i += 3) {
		RFloat v1[3] = { 0.0f };
		RFloat v2[3] = { 0.0f };
		RFloat normal[3] = { 0.0f };

		//p1-p0
		v1[0] = vs[3 * id[i + 1]] - vs[3 * id[i]];
		v1[1] = vs[3 * id[i + 1] + 1] - vs[3 * id[i] + 1];
		v1[2] = vs[3 * id[i + 1] + 2] - vs[3 * id[i] + 2];

		//p2-p1
		v2[0] = vs[3 * id[i + 2]] - vs[3 * id[i + 1]];
		v2[1] = vs[3 * id[i + 2] + 1] - vs[3 * id[i + 1] + 1];
		v2[2] = vs[3 * id[i + 2] + 2] - vs[3 * id[i + 1] + 2];

		//v1*v2
		normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
		normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
		normal[2] = v1[0] * v2[1] - v1[1] * v2[0];

		//单位化
		RFloat len = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
		normal[0] /= len, normal[1] /= len, normal[2] /= len;

		for (int j = 0; j < 3; j++) {
			nm[3 * id[i + j] + 0] += normal[0];
			nm[3 * id[i + j] + 1] += normal[1];
			nm[3 * id[i + j] + 2] += normal[2];
		}
	}

	//单位化
	for (int i = 0; i < vSize; i += 3) {
		float len = sqrt(nm[i] * nm[i] + nm[i + 1] * nm[i + 1] + nm[i + 2] * nm[i + 2]);
		nm[i] /= len;
		nm[i + 1] /= len;
		nm[i + 2] /= len;
	}
}

//根据vertices和indices计算tangents
void ResourceManager::_calcTangent(RFloat* vs, RInt vSize, RUInt* id, RInt iSize, RFloat* uv, RFloat* tg) {
	for (int i = 0; i < vSize; i++) {
		tg[i] = 0.0f;
	}
	for (int i = 0; i < iSize; i += 3) {
		RFloat e1[3] = { 0.0f };
		RFloat e2[3] = { 0.0f };
		RFloat duv1[2] = { 0.0f };
		RFloat duv2[2] = { 0.0f };
		RFloat tangent[3] = { 0.0f };

		//e1 = p1 - p0
		e1[0] = vs[3 * id[i + 1]] - vs[3 * id[i]];
		e1[1] = vs[3 * id[i + 1] + 1] - vs[3 * id[i] + 1];
		e1[2] = vs[3 * id[i + 1] + 2] - vs[3 * id[i] + 2];

		//e2 = p2 - p0
		e2[0] = vs[3 * id[i + 2]] - vs[3 * id[i]];
		e2[1] = vs[3 * id[i + 2] + 1] - vs[3 * id[i] + 1];
		e2[2] = vs[3 * id[i + 2] + 2] - vs[3 * id[i] + 2];

		//duv1 = uv1 - uv0
		duv1[0] = uv[2 * id[i + 1]] - uv[2 * id[i]];
		duv1[1] = uv[2 * id[i + 1] + 1] - uv[2 * id[i] + 1];

		//duv2 = uv2 - uv0
		duv2[0] = uv[2 * id[i + 2]] - uv[2 * id[i]];
		duv2[1] = uv[2 * id[i + 2] + 1] - uv[2 * id[i] + 1];

		//tangent
		RFloat f = 1.0f / duv1[0] * duv2[1] - duv2[0] * duv1[1];
		tangent[0] = f * (duv2[1] * e1[0] - duv1[1] * e2[0]);
		tangent[1] = f * (duv2[1] * e1[1] - duv1[1] * e2[1]);
		tangent[2] = f * (duv2[1] * e1[2] - duv1[1] * e2[2]);

		//normalize
		RFloat len = sqrt(tangent[0] * tangent[0] + tangent[1] * tangent[1] + tangent[2] * tangent[2]);
		tangent[0] /= len, tangent[1] /= len, tangent[2] /= len;

		for (int j = 0; j < 3; j++) {
			tg[3 * id[i + j] + 0] += tangent[0];
			tg[3 * id[i + j] + 1] += tangent[1];
			tg[3 * id[i + j] + 2] += tangent[2];
		}
	}

	//单位化
	for (int i = 0; i < vSize; i += 3) {
		float len = sqrt(tg[i] * tg[i] + tg[i + 1] * tg[i + 1] + tg[i + 2] * tg[i + 2]);
		tg[i] /= len;
		tg[i + 1] /= len;
		tg[i + 2] /= len;
	}
}


//清空所有网格资源
void ResourceManager::clearMesh() {
	if (!meshPool.empty()) {
		for (auto iter : meshPool) {
			if (iter.second != nullptr) {
				Mesh::Destroy(iter.second);
				iter.second = nullptr;
			}
		}
		meshPool.clear();
	}
}
//清空所有材质资源
void ResourceManager::clearMaterial() {
	if (!materialPool.empty()) {
		for (auto iter : materialPool) {
			if (iter.second != nullptr) {
				Material::Destroy(iter.second);
				iter.second = nullptr;
			}
		}
		materialPool.clear();
	}
}
//清空所有纹理资源
void ResourceManager::clearTexture() {
	if (!texturePool.empty()) {
		for (auto iter : texturePool) {
			if (iter.second != nullptr) {
				Texture::Destroy(iter.second);
				iter.second = nullptr;
			}
		}
		texturePool.clear();
	}
}
//清空所有shader
void ResourceManager::clearShader() {
	if (!shaderPool.empty()) {
		for (auto iter : shaderPool) {
			if (iter.second != nullptr) {
				GLProgram::Destroy(iter.second);
				iter.second = nullptr;
			}
		}
		shaderPool.clear();
	}
}
//清空所有资源
void ResourceManager::clearAll() {
	clearMesh();
	clearMaterial();
	clearTexture();
	clearShader();
}

//加载模型
RGameObject* ResourceManager::loadModel(RString path, RScene* sce) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return nullptr;
	}
	RGameObject* go = new RGameObject(sce);
	CRender* render = (CRender*)go->addComponent(RENDER);

	//获取文件目录
	int n;
	RString modelDir;
	if ((n = path.find_last_of('/')) != RString::npos) {
		modelDir = path.substr(0, n + 1);
	}
	_processNode(scene->mRootNode, scene, render, modelDir);
	return go;
}

//加载模型时处理节点
void ResourceManager::_processNode(aiNode* node, const aiScene* scene, CRender* render, RString modelDir) {
	for (RUInt i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_processMesh(mesh, scene, render, modelDir);
	}
	for (RUInt i = 0; i < node->mNumChildren; i++) {
		_processNode(node->mChildren[i], scene, render, modelDir);
	}
}

//加载模型时处理网格
void ResourceManager::_processMesh(aiMesh* mesh, const aiScene* scene, CRender* render, RString modelDir) {
	//读取vertex position normal tangant color uv
	int pointCount = mesh->mNumVertices;
	RInt verSize = 14 * pointCount;
	RFloat* verArray = new RFloat[verSize];
	for (int i = 0; i < pointCount; i++) {
		//position
		verArray[14 * i] = mesh->mVertices[i].x;
		verArray[14 * i + 1] = mesh->mVertices[i].y;
		verArray[14 * i + 2] = mesh->mVertices[i].z;
		//normal
		verArray[14 * i + 3] = mesh->mNormals[i].x;
		verArray[14 * i + 4] = mesh->mNormals[i].y;
		verArray[14 * i + 5] = mesh->mNormals[i].z;
	}
	//tangent
	if (mesh->mTangents) {
		for (int i = 0; i < pointCount; i++) {
			verArray[14 * i + 6] = mesh->mTangents[i].x;
			verArray[14 * i + 7] = mesh->mTangents[i].y;
			verArray[14 * i + 8] = mesh->mTangents[i].z;
		}
	}
	else {
		for (int i = 0; i < pointCount; i++) {
			verArray[14 * i + 6] = 0.0f;
			verArray[14 * i + 7] = 0.0f;
			verArray[14 * i + 8] = 0.0f;
		}
	}
	//uv
	if (mesh->mTextureCoords[0]) {
		for (int i = 0; i < pointCount; i++) {
			verArray[14 * i + 12] = mesh->mTextureCoords[0][i].x;
			verArray[14 * i + 13] = mesh->mTextureCoords[0][i].y;
		}
	}
	else {
		for (int i = 0; i < pointCount; i++) {
			verArray[14 * i + 12] = 0.0f;
			verArray[14 * i + 13] = 0.0f;
		}
	}
	//color
	if (mesh->mColors[0]) {
		for (int i = 0; i < pointCount; i++) {
			verArray[14 * i + 9] = mesh->mColors[i]->r;
			verArray[14 * i + 10] = mesh->mColors[i]->g;
			verArray[14 * i + 11] = mesh->mColors[i]->b;
		}
	}
	else {
		for (int i = 0; i < pointCount; i++) {
			verArray[14 * i + 9] = 0.0f;
			verArray[14 * i + 10] = 0.0f;
			verArray[14 * i + 11] = 0.0f;
		}
	}
	//读取indices
	RUInt faceCount = mesh->mNumFaces;
	RInt indSize = 3 * faceCount;
	RUInt* indArray = new RUInt[indSize];
	for (RUInt i = 0; i < faceCount; i++) {
		aiFace face = mesh->mFaces[i];
		indArray[3 * i] = face.mIndices[0];
		indArray[3 * i + 1] = face.mIndices[1];
		indArray[3 * i + 2] = face.mIndices[2];
	}
	RString meshName = mesh->mName.C_Str();

	render->addMesh(createMesh(meshName, verArray, verSize, indArray, indSize));

	Material* mat = createMaterial(meshName + "_mat");
	mat->setShader("phong");
	mat->setColor(1.0f, 1.0f, 1.0f);
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
		_loadMaterialTexture(mat, aiMat, modelDir);
	}
	render->addMaterial(mat);

	delete[] verArray;
	delete[] indArray;
	//render->setMesh(getMesh("plane"));
}

//加载模型得材质纹理
void ResourceManager::_loadMaterialTexture(Material* mat, aiMaterial* aiMat, RString modelDir) {
	for (unsigned int i = 0; i < aiMat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString str;
		aiMat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		Texture* tex = createTexture(modelDir + str.C_Str());
		mat->addTexture(tex);
	}
}