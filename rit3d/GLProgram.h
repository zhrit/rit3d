#pragma once

#include "RCommon.h"

class GLProgram {
public:
	static GLProgram* Create(const RString& _name, const RString& vertexPath, const RString& fragmentPath, const RString& defines);
	static void Destroy(GLProgram* _shader);
private:
	GLProgram(const RString& _name, const RString& vertexPath, const RString& fragmentPath, const RString& defines);
	virtual ~GLProgram();

	RString m_name;

	void _destroy();
public:
	RUInt ID;

	void use();

	void setBool(const RString &name, RBool value) const;

	void setInt(const RString &name, RInt value) const;

	void setFloat(const RString &name, RFloat value) const;

	void setVec3(const RString &name, RFloat x, RFloat y, RFloat z) const;

	void setVec3(const RString &name, glm::vec3 value) const;

	void setMat4(const RString &name, glm::mat4 value) const;
};
