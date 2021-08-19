#pragma once
#include "includes.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
//#include "game.h"

using namespace std;

class HUD {
public:
	HUD(int sw, int sh);
	~HUD();

	Camera* cam;
	Shader* tex_shader;
	
	//Minimap
	Mesh* minimap_back_mesh;
	Texture* minimap_back_tex;
	Texture* minimap_back_tex_alpha;

	void render();
	void update(float elapsed_time);
};