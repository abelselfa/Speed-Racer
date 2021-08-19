#pragma once
#include "GameObjectMesh.h"

class GameObjectAsteroid : public GameObjectMesh {
public:

	Vector3 orbitCenter;
	float orbitRadius;
	Vector3 rotationAxis;

	void update(float elapsed_time);
};