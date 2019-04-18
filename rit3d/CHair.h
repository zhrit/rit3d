#pragma once
#include "RCommon.h"
#include "IComponent.h"
#include "GLProgram.h"

struct HairNode {
	glm::vec3 p0;//前一帧的位置
	glm::vec3 p1;//本帧的位置
	RFloat maxLength;//和上一个节点的止动长度
};

struct HairStrand {
	int nodeStart, nodeEnd;//发束所包含的起始和末尾节点的索引
	glm::vec3 rootP;//发根的位置
};

class CHair : public IComponent
{
private:
	CHair();
	virtual ~CHair();

	void _init();
public:
	RUInt m_VAO;
	RUInt m_VBO;
	std::vector<HairNode> m_nodes;//所有节点
	std::vector<glm::vec3> m_drawData;//绘制需要的数据
	std::vector<HairStrand> m_strands;//所有发束
	glm::vec3 a{ 0.0f, -9.8f, 0.0f };//加速度
	RFloat d{ 1.0f };//阻尼
	glm::vec3 center{ 0.0f, 0.0f, 0.0f };//碰撞球中心
	RFloat raduis{ 0.5f };//碰撞球半径
	RInt nodeInStrand{ 7 };//一个发束中的节点数
	RFloat minLength{ 0.05f };//发束第一段的长度

	GLProgram* shader;
	static CHair* CreateInstance();
};

