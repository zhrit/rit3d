#pragma once

#include "RCommon.h"

class RWindow {
public:
	RWindow();

	RWindow(RString title);

	RWindow(RString title, RInt x, RInt y, RInt width, RInt height);

	virtual ~RWindow();
private:
	RString m_title{ "Rit3D" };
	RInt m_xPos{ 0 }, m_yPos{ 0 };
	RInt m_width{ 800 }, m_height{ 600 };
	GLFWwindow* m_window{ nullptr };

public:
	void setTitle(RString t);
	RString getTitle() const;

	RInt getWidth() const;
	void setWidth(RInt width);

	RInt getHeight() const;
	void setHeight(RInt height);

	void setPosition(RInt x, RInt y);
	RInt getX() const;
	RInt getY() const;

	GLFWwindow* getWindowContext() const;

	RBool RWindowShouldClose();

	void swapBuffer();

	void pollEvents();

	void destroy();
public:
	void initWindow();
};
