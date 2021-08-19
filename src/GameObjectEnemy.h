#pragma once
#include "GameObjectMesh.h"
#include "GameObjectAsteroid.h"

class GameObjectEnemy : public GameObjectMesh {
public:
	const static int ENEMY1 = 1;
	const static int ENEMY2 = 2;

	int enemyType;
	int currentAro;

	void update(float elapsed_time);
	void dodgeAsteroid(GameObjectAsteroid asteroid);
};