#pragma once
#include "GameObject.h"
#include "HUD.h"
#include "Menu.h"

class Scene {
public:
	Scene();
	~Scene();

	void getColliders(std::vector<GameObject*>& colliders, GameObject* current);

	GameObject* root;
	HUD* hud;
	Menu* menu;

	vector<GameObject*> listOfAros;
	int numberOfAros;
};