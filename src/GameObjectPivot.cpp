#include "GameObjectPivot.h"

/*void GameObjectCollider::render() {
	//Do nothing, cause this GameObject doesn't have to render
}*/

void GameObjectPivot::update(float elapsed_time) {

	//this->model = parent->model * this->model;


	/*
	Matrix44 aux_scale = parent->model;
	aux_scale.m[0] = this->model.m[0];
	aux_scale.m[5] = this->model.m[5];
	aux_scale.m[10] = this->model.m[10];

	this->model = aux_scale; */




	//this->model = this->model * parent->model;

	//Game* game = Game::instance;
	//cout << to_string(this->model.position().x) + "-x-" << to_string(this->model.position().y) + "-y-" << to_string(this->model.position().z) + "-z-";
	//cout << to_string(game->playerPosition.x) + "-x-";
	//this->model.traslate(game->playerPosition.x, game->playerPosition.y, game->playerPosition.z);
	//skybox->model.setScale(10, 10, 10);

	//this->model.setTranslation(game->playerPosition.x, game->playerPosition.y, game->playerPosition.z);

	//if (this->model.position().x == game->playerPosition.x) {
		//cout << to_string(game->playerPosition.x) + "-x-";
		//cout << to_string(this->model.position().x) + "-x-";
		//cout << "yes";
	//}
	//cout << to_string(game->playerPosition.x) + "-x-";
	//cout << to_string(this->model.position().x) + "-x-";
	GameObject::update(elapsed_time);
}