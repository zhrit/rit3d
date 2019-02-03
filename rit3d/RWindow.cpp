#include "pch.h"
#include "RWindow.h"

RWindow::RWindow() {
	initWindow();
}

RWindow::RWindow(RString title) : m_title(title) {
	initWindow();
}

RWindow::RWindow(RString title, RInt x, RInt y, RInt width, RInt height)
	: m_title(title), m_xPos(x), m_yPos(y), m_width(width), m_height(height){
	initWindow();
}

RWindow::~RWindow() {

}

void RWindow::setTitle(RString t) {
	m_title = t;
}

RString RWindow::getTitle() const {
	return m_title;
}

RInt RWindow::getWidth() const {
	return m_width;
}
void RWindow::setWidth(int width) {
	m_width = width;
}

RInt RWindow::getHeight() const {
	return m_height;
}
void RWindow::setHeight(int height) {
	m_height = height;
}

void RWindow::setPosition(int x, int y) {
	m_xPos = x;
	m_yPos = y;
}
RInt RWindow::getX() const {
	return m_xPos;
}
RInt RWindow::getY() const {
	return m_yPos;
}

GLFWwindow* RWindow::getWindowContext() const {
	return m_window;
}

void RWindow::initWindow() {
	//³õÊ¼»¯GLFW´°¿Ú
	if (!glfwInit()) {
		std::cout << "Failed to init glfw" << std::endl;
		return;
	}
	m_window = glfwCreateWindow(800, 600, "Rit3D", NULL, NULL);
	if (m_window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);
}

RBool RWindow::RWindowShouldClose() {
	return glfwWindowShouldClose(m_window);
}

void RWindow::swapBuffer() {
	glfwSwapBuffers(m_window);
}

void RWindow::pollEvents() {
	glfwPollEvents();
}

void RWindow::destroy() {
	glfwTerminate();
}
