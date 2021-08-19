#pragma once
#include "GameObjectMesh.h"

class GameObjectSkybox : public GameObjectMesh {
public:
	void render();
	void update(float elapsed_time);
};