#pragma once
#include "RCommon.h"

class RGameObject;
class BaseBehavior {
public:
	BaseBehavior();
	virtual ~BaseBehavior();

	//创建组件后的下一帧执行
	virtual void onStart();
	
	//每帧执行
	virtual void onUpdate();

	//每帧执行，在onUpdate之后
	virtual void onLateUpdate();

	//碰撞时执行
	virtual void onCollide();

	void update();

	void lateUpdate();

	RGameObject* gameObject;

private:
	RBool m_started{ false };
};