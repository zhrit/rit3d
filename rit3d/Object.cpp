#include "pch.h"
#include "Object.h"
#include "Application.h"

Object::Object() {
	id = Application::Instance()->IDAllocator();
}

Object::~Object() {

}