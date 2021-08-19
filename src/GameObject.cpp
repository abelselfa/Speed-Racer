#include "GameObject.h"

void GameObject::addChild(GameObject* obj) {

	obj->parent = this;

	this->children.push_back(obj);
}

Matrix44 GameObject::getGlobalMatrix() {
	if (this->parent) {
		return this->parent->getGlobalMatrix() * this->model;
	}
	else {
		return this->model;
	}
}

void GameObject::render() {

	for (size_t i = 0; i < children.size(); i++) {
		children[i]->render();
	}
}

void GameObject::update(float elapsed_time) {

	for (size_t i = 0; i < children.size(); i++) {
		children[i]->update(elapsed_time);
	}
}