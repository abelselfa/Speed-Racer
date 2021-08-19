#pragma once
#include "GameObjectMesh.h"

class GameObjectAro : public GameObjectMesh {
public:
	int aro_surpassed;
	float scale;
	int position;

	void update(float elapsed_time);
};