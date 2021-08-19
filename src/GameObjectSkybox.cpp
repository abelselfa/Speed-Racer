#include "GameObjectSkybox.h"

void GameObjectSkybox::render() {
	Game* g = Game::instance;

	//create model matrix for our plane mesh

	shader->enable();

	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	//create our mvp
	Matrix44 mvp = this->getGlobalMatrix() * g->camera->viewprojection_matrix;

	Matrix44 inverse_m = model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();

	//light position
	Vector3 light_pos(g->camera->eye);

	shader->setUniform3("u_eye", g->camera->eye);
	shader->setMatrix44("u_m", model);
	shader->setMatrix44("u_mvp", mvp);
	shader->setMatrix44("u_normal_matrix", normal_m);
	shader->setUniform3("u_light", light_pos);
	shader->setTexture("u_texture_diffuse", texture);
	shader->setFloat("u_current_speed", current_speed);
	shader->setFloat("u_base_speed", base_speed);

	//resource_manager.getMesh("x3_fighter")->render(GL_TRIANGLES, shader);
	this->mesh->render(GL_TRIANGLES, this->shader);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	//disable the shader
	shader->disable();

	GameObject::render();
}

void GameObjectSkybox::update(float elapsed_time) {

	Game* game = Game::instance;
	//cout << to_string(game->playerPosition.x) + "-x-" << to_string(game->playerPosition.y) + "-y-" << to_string(game->playerPosition.z) + "-z-";
	//cout << to_string(game->playerPosition.x) + "-x-";
	//this->model.traslate(game->playerPosition.x, game->playerPosition.y, game->playerPosition.z);
	//skybox->model.setScale(10, 10, 10);

	this->model.setTranslation(game->playerPosition.x, game->playerPosition.y, game->playerPosition.z);

	//if (this->model.position().x == game->playerPosition.x) {
		//cout << to_string(game->playerPosition.x) + "-x-";
		//cout << to_string(this->model.position().x) + "-x-";
		//cout << "yes";
	//}
	//cout << to_string(game->playerPosition.x) + "-x-";
	//cout << to_string(this->model.position().x) + "-x-";
	GameObject::update(elapsed_time);
}