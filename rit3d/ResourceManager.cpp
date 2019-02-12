#include "pch.h"
#include "ResourceManager.h"

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
	Material* mat = Material::Create(_name);
	std::pair<std::map<RString, Material*>::iterator, bool> Insert_Pair;

	Insert_Pair = materialPool.insert(std::pair<RString, Material*>(_name, mat));

	if (Insert_Pair.second == false) {
		Material::Destroy(mat);
		cout << "已有同名材质，创建失败" << endl;
		return nullptr;
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
		cout << "材质资源不存在！" << endl;
		return nullptr;
	}
}

//创建新纹理
Texture* ResourceManager::createTexture(const RString& imagePath) {
	Texture* tex = Texture::Create(imagePath);
	std::pair<std::map<RString, Texture*>::iterator, bool> Insert_Pair;

	Insert_Pair = texturePool.insert(std::pair<RString, Texture*>(imagePath, tex));

	if (Insert_Pair.second == false) {
		Texture::Destroy(tex);
		cout << "已有同名纹理，创建失败" << endl;
		return nullptr;
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
		cout << "纹理资源不存在！" << endl;
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
		RFloat vertices_quad[] = {
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		};
		RUInt indices_quad[] = {
			0, 2, 1, 0, 3, 2
		};
		createMesh("quad", vertices_quad, 4 * 11, indices_quad, 6);
	}
	{
		//plane
		RFloat vertices_plane[1331];//121*11
		int k = 0;
		for (int i = -5; i <= 5; i++) {
			for (int j = -5; j <= 5; j++) {
				vertices_plane[k * 11] = (RFloat)j;
				vertices_plane[k * 11 + 1] = 0.0f;
				vertices_plane[k * 11 + 2] = (RFloat)i;
				vertices_plane[k * 11 + 3] = 0.0f;
				vertices_plane[k * 11 + 4] = 1.0f;
				vertices_plane[k * 11 + 5] = 0.0f;
				vertices_plane[k * 11 + 6] = 1.0f;
				vertices_plane[k * 11 + 7] = 1.0f;
				vertices_plane[k * 11 + 8] = 1.0f;
				vertices_plane[k * 11 + 9] = ((RFloat)j + 5.0f)*0.1f;
				vertices_plane[k * 11 + 10] = (5.0f - (RFloat)i) * 0.1f;
				k++;
			}
		}
		k = 0;
		RUInt indices_plane[600];
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
		createMesh("plane", vertices_plane, 1331, indices_plane, 600);
	}
	{
		//cube
		float vertices_cube[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f
		};
		RUInt indices_cube[] = {
			0,2,1,0,3,2,4,6,5,4,7,6,8,10,9,8,11,10,12,14,13,12,15,14,16,18,17,16,19,18,20,22,21,20,23,22
		};
		createMesh("cube", vertices_cube, 24 * 11, indices_cube, 36);
	}
	{
		//sphere
		const RUInt X_SEGMENTS = 64;
		const RUInt Y_SEGMENTS = 64;
		//4225个点,46475
		RFloat vertices_sphere[46475] = { 0.0f };
		RUInt i = 0;
		for (RUInt y = 0; y <= Y_SEGMENTS; y++) {
			for (RUInt x = 0; x <= X_SEGMENTS; x++) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = 0.5f * cos(xSegment * PI2) * sin(ySegment * PI);
				float yPos = 0.5f * cos(ySegment * PI);
				float zPos = 0.5f * sin(xSegment * PI2) * sin(ySegment * PI);
				vertices_sphere[i * 11] = xPos;
				vertices_sphere[i * 11 + 1] = yPos;
				vertices_sphere[i * 11 + 2] = zPos;
				vertices_sphere[i * 11 + 3] = 2.0f * xPos;
				vertices_sphere[i * 11 + 4] = 2.0f * yPos;
				vertices_sphere[i * 11 + 5] = 2.0f * zPos;
				vertices_sphere[i * 11 + 6] = 1.0f;
				vertices_sphere[i * 11 + 7] = 1.0f;
				vertices_sphere[i * 11 + 8] = 1.0f;
				vertices_sphere[i * 11 + 9] = xSegment;
				vertices_sphere[i * 11 + 10] = ySegment;
				i++;
			}
		}
		RUInt indices_sphere[24576] = { 0 };
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
		createMesh("sphere", vertices_sphere, 46475, indices_sphere, 24576);
	}
	{
		//cone 60份 122个点
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

		RFloat vertices_cone[1342];
		for (int i = 0; i < 122; i++) {
			vertices_cone[i * 11] = vertices[i * 3];
			vertices_cone[i * 11 + 1] = vertices[i * 3 + 1];
			vertices_cone[i * 11 + 2] = vertices[i * 3 + 2];
			vertices_cone[i * 11 + 3] = normals[i * 3];
			vertices_cone[i * 11 + 4] = normals[i * 3 + 1];
			vertices_cone[i * 11 + 5] = normals[i * 3 + 2];
			vertices_cone[i * 11 + 6] = 1.0f;
			vertices_cone[i * 11 + 7] = 1.0f;
			vertices_cone[i * 11 + 8] = 1.0f;
			vertices_cone[i * 11 + 9] = uv[i * 2];
			vertices_cone[i * 11 + 10] = uv[i * 2] + 1;
		}
		createMesh("cone", vertices_cone, 1342, indices_cone, 360);
	}
	{
		//cylinder // 60份 244个点
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

		RUInt indices_cylinder[720];
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

		RFloat vertices_cylinder[2684];
		for (int i = 0; i < 244; i++) {
			vertices_cylinder[i * 11] = vertices[i * 3];
			vertices_cylinder[i * 11 + 1] = vertices[i * 3 + 1];
			vertices_cylinder[i * 11 + 2] = vertices[i * 3 + 2];
			vertices_cylinder[i * 11 + 3] = normals[i * 3];
			vertices_cylinder[i * 11 + 4] = normals[i * 3 + 1];
			vertices_cylinder[i * 11 + 5] = normals[i * 3 + 2];
			vertices_cylinder[i * 11 + 6] = 1.0f;
			vertices_cylinder[i * 11 + 7] = 1.0f;
			vertices_cylinder[i * 11 + 8] = 1.0f;
			vertices_cylinder[i * 11 + 9] = uv[i * 2];
			vertices_cylinder[i * 11 + 10] = uv[i * 2] + 1;
		}
		createMesh("cylinder", vertices_cylinder, 2684, indices_cylinder, 720);
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