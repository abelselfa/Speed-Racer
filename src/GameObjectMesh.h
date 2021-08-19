#pragma once
#include "GameObject.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "game.h"
#include "coldet.h"

class GameObjectMesh : public GameObject {
public:
	/*const static int PLAYER = 1;
	const static int PROP = 2;*/
	//const static int SKYBOX = 3;
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector3 color;
	//int objectType;
	float base_speed = 0;
	float current_speed = 0;

	CollisionModel3D* collision_model;
	void setCollisionModel();

	void render();
};