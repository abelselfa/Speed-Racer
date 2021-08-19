#pragma once
#include "GameObjectMesh.h"

class GameObjectPlayer : public GameObjectMesh {
public:
	//const static int PLAYER = 1;
	int currentAro = 0;
	float pitch_speed = 1.5f;
	float roll_speed = 3.0f;
	float max_free_spin_speed_x = 100;
	float max_free_spin_speed_y = 160;

	void update(float elapsed_time);
};