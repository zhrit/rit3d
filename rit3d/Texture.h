#pragma once
#include "RCommon.h"

class Texture {
public:
	static Texture* Create(RString imagePath);
	static void Destroy(Texture* _tex);

private:
	Texture(RString imagePath);
	~Texture();

	GLTexture m_texture{ 0 };

	void _destroy();
public:
	//获取gltexture对象的id
	GLTexture getTexture() const;
	//使用该texture
	void use(RUInt unitId) const;
};