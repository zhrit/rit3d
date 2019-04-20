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
	std::vector<RFloat> scalp_v;//头皮顶点
	std::vector<RUInt> scalp_i;//头皮索引
	RUInt m_VAO;
	RUInt m_VBO;
	std::vector<HairNode> m_nodes;//所有节点
	RFloat* m_drawData;//绘制需要的数据。注意：注意：注意：指针操作比vector的push_back操作快很多很多，大量数据处理用指针操作
	RUInt m_dataSize;//drawData的大小;
	RUInt m_pointer4Data;//drawData的数据末尾
	std::vector<HairStrand> m_strands;//所有发束
	glm::vec3 a{ 0.0f, -9.8f, 0.0f };//加速度
	RFloat d{ 1.0f };//阻尼
	glm::vec3 center{ 0.0f, 0.0f, 0.0f };//碰撞球中心
	RFloat raduis{ 0.5f };//碰撞球半径
	RInt nodeInStrand{ 7 };//一个发束中的节点数
	RFloat minLength{ 0.05f };//发束第一段的长度
	RUInt interN{ 4 };//插值点的个数
	RUInt strandN{ 10 };//一个三角形面中插入的头发数
	std::vector<glm::vec3> m_interCoord;//插值的重心坐标

	GLProgram* shader;
	static CHair* CreateInstance();
};

