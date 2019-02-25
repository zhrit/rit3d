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

	//窗口大小改变时执行
	virtual void onChangeSize(int _w, int _h);
	//鼠标事件
	virtual void onLeftButtonDown();
	virtual void onLeftButtonUp();
	virtual void onRightButtonDown();
	virtual void onRightButtonUp();
	virtual void onMouseMove(double _x, double _y);
	virtual void onScroll(double _x, double _y);
	//键盘事件
	virtual void onKeyDown(int key);
	virtual void onKeyKeep(int key);
	virtual void onKeyUp(int key);

	void update();

	void lateUpdate();

	RGameObject* gameObject;

private:
	RBool m_started{ false };
};