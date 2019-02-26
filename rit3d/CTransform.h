#pragma once
#include "RCommon.h"
#include "IComponent.h"

class CTransform : public IComponent {
private:
	CTransform();
	virtual ~CTransform();

	//几何信息
	glm::vec3 m_localPosition{ 0.0f,0.0f,0.0f };  //局部坐标系 位置坐标
	glm::vec3 m_localAngle{ 0.0f,0.0f,0.0f };     //局部坐标系 各轴旋转角度
	glm::vec3 m_localUpDir{ 0.0f, 1.0f, 0.0f };   //上向量(+y)
	glm::vec3 m_localFrontDir{ 0.0f, 0.0f, 1.0f };//前向量(+z)
	glm::vec3 m_localRightDir{ 1.0f, 0.0f, 0.0f };//右向量(+x)
	glm::vec3 m_localScale{ 1.0f, 1.0f,1.0f };    //局部坐标系 缩放

	glm::mat4 m_modelMatrix{ 1.0f };              //全局模型矩阵
	glm::mat4 m_localModelMatrix{ 1.0f };         //局部模型矩阵
	glm::mat4 m_viewMatrix{ 1.0f };               //视图矩阵

	RBool m_localModelDirty{ true };              //局部几何信息标脏
	RBool m_modelDirty{ true };                   //全局几何信息标脏

	//相机和灯光暂时是考虑放在场景中，不考虑嵌套
	RBool m_viewDirty{ true };                    //视图矩阵标脏
	//针对点光源阴影
	RBool m_viewsDirty{ true };
	std::vector<glm::mat4> m_viewsMatrix;

public:
	static CTransform* CreateInstance();

public:
	CTransform* parent;
	std::vector<CTransform*> children;

	//设置可用状态
	void setEnabled(RBool value);

	//设置父对象
	CTransform* getParent() const;
	RBool setParent(CTransform* p);

	//局部获取几何信息
	glm::vec3 getLocalPosition() const;
	glm::vec3 getLocalAngle() const;
	glm::vec3 getLocalScale() const;
	glm::vec3 getLocalUpDir();
	glm::vec3 getLocalFrontDir();
	glm::vec3 getLocalRightDir();

	//局部设置几何信息
	void setLocalPosition(float x, float y, float z);
	void setLocalAngle(float x, float y, float z);
	void setLocalScale(float x, float y, float z);
	void setLocalUpDir(float x, float y, float z);
	void setLocalFrontDir(float x, float y, float z);

	//全局获取几何信息
	glm::vec3 getPosition();
	glm::vec3 getAngle();
	glm::vec3 getScale();

	//局部获取模型矩阵
	glm::mat4 getLocalModelMatrix();
	//全局获取模型矩阵
	glm::mat4 getModelMatrix();
	//获取视图矩阵
	glm::mat4 getViewMatrix();
	//获取六个视图矩阵，主要是为了点光源阴影
	std::vector<glm::mat4> getViewMatrixs();

private:
	//局部模型矩阵标脏
	void _setLocalDirty();
	//全局模型矩阵标脏
	void _setDirty();
};

