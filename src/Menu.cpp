#include "Menu.h"
#include "game.h"

Menu::Menu(int sw, int sh) {

	Game* g = Game::instance;

	cam = new Camera();
	cam->setOrthographic(0, sw, 0, sh, 0.1, 2);
	cam->updateViewMatrix();

	menu_mesh = new Mesh();
	menu_mesh->vertices.resize(4);
	menu_mesh->uvs.resize(4);

	menu_mesh->vertices[0] = Vector3(0, 0, 1);
	menu_mesh->vertices[1] = Vector3(sw, 0, 1);
	menu_mesh->vertices[2] = Vector3(sw, sh, 1);
	menu_mesh->vertices[3] = Vector3(0, sh, 1);

	menu_mesh->uvs[0] = Vector2(0, 0);
	menu_mesh->uvs[1] = Vector2(1, 0);
	menu_mesh->uvs[2] = Vector2(1, 1);
	menu_mesh->uvs[3] = Vector2(0, 1);

	menu_mesh->indices.resize(6);
	menu_mesh->indices[0] = 0;
	menu_mesh->indices[1] = 1;
	menu_mesh->indices[2] = 2;
	menu_mesh->indices[3] = 0;
	menu_mesh->indices[4] = 2;
	menu_mesh->indices[5] = 3;

	menu_mesh->genBuffers();

	tex_shader = g->resource_manager.getShader("menu");
	loading_tex = g->resource_manager.getTexture("loading_screen");
}

void Menu::renderMenu(int menuOption) {

	Game* g = Game::instance;

	glDisable(GL_DEPTH_TEST);

	tex_shader->enable();
	switch (menuOption) {
	case 0:
		loading_tex = g->resource_manager.getTexture("menu_play");
		tex_shader->setTexture("u_texture_diffuse", loading_tex);
		break;
	case 1:
		loading_tex = g->resource_manager.getTexture("menu_options");
		tex_shader->setTexture("u_texture_diffuse", loading_tex);
		break;
	case 2:
		loading_tex = g->resource_manager.getTexture("menu_exit");
		tex_shader->setTexture("u_texture_diffuse", loading_tex);
		break;
	}
	tex_shader->setTexture("u_texture_diffuse", loading_tex);
	tex_shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	menu_mesh->render(GL_TRIANGLES, tex_shader);
	tex_shader->disable();

	glEnable(GL_DEPTH_TEST);
}

void Menu::renderLoading() {

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	tex_shader->enable();
	tex_shader->setTexture("u_texture_diffuse", loading_tex);
	tex_shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	menu_mesh->render(GL_TRIANGLES, tex_shader);
	tex_shader->disable();

	glEnable(GL_DEPTH_TEST);
}

void Menu::renderFinalScreen() {

	Game* g = Game::instance;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	tex_shader->enable();

	switch (g->is_winner) {
	case true:
		loading_tex = g->resource_manager.getTexture("victory");
		tex_shader->setTexture("u_texture_diffuse", loading_tex);
		break;
	case false:
		loading_tex = g->resource_manager.getTexture("defeat");
		tex_shader->setTexture("u_texture_diffuse", loading_tex);
		break;
	}

	tex_shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	menu_mesh->render(GL_TRIANGLES, tex_shader);
	tex_shader->disable();

	glEnable(GL_DEPTH_TEST);
}