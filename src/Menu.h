#pragma once
#include "includes.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
//#include "game.h"

using namespace std;

class Menu {
public:
	Menu(int sw, int sh);
	~Menu();

	Camera* cam;
	Shader* tex_shader;

	//Minimap
	Mesh* menu_mesh;
	Texture* loading_tex;

	void renderMenu(int menuOption);
	void renderLoading();
	void renderFinalScreen();
};