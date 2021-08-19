#pragma once
#include "includes.h"
#include "framework.h"
#include <string>

using namespace std;

class GameObject {
public:
	string name;
	Matrix44 model;

	bool is_collider = false;

	GameObject* parent = nullptr;
	vector<GameObject*> children;

	void addChild(GameObject* obj);

	Matrix44 getGlobalMatrix();

	virtual void render(); //el virtual permite sobreescribir la funcion en una subclase
	virtual void update(float elapsed_time);
};