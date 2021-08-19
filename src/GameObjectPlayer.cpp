#include "GameObjectPlayer.h"
#include "GameObjectAro.h"
#include "GameObjectEnemy.h"

void GameObjectPlayer::update(float elapsed_time) {

	Game* game = Game::instance;

	if (game->keystate[SDL_SCANCODE_LSHIFT] && current_speed < (base_speed * 2.5f)) {

		current_speed *= 1.01f; //move faster with left shift
	}
	else {

		if (current_speed > base_speed) {

			current_speed /= 1.02f;
		}
	}

	float speed_dt = elapsed_time * current_speed;

	if (game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP]) {

		this->model.rotateLocal(elapsed_time * pitch_speed, Vector3(-1, 0, 0));
	}

	if (game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN]) {

		this->model.rotateLocal(elapsed_time * pitch_speed, Vector3(1, 0, 0));
	}

	if (game->keystate[SDL_SCANCODE_A] || game->keystate[SDL_SCANCODE_LEFT]) {

		this->model.rotateLocal(elapsed_time * roll_speed, Vector3(0, 0, -1));
	}

	if (game->keystate[SDL_SCANCODE_D] || game->keystate[SDL_SCANCODE_RIGHT]) {

		this->model.rotateLocal(elapsed_time * roll_speed, Vector3(0, 0, 1));
	}

	if (game->mouse_delta.x > max_free_spin_speed_x) {

		game->mouse_delta.x = max_free_spin_speed_x;
	}

	if (game->mouse_delta.x < -max_free_spin_speed_x) {

		game->mouse_delta.x = -max_free_spin_speed_x;
	}

	if (game->mouse_delta.y > max_free_spin_speed_y) {

		game->mouse_delta.y = max_free_spin_speed_y;
	}

	if (game->mouse_delta.y < -max_free_spin_speed_y) {

		game->mouse_delta.y = -max_free_spin_speed_y;
	}

	this->model.rotateLocal(game->mouse_delta.x * 0.0005f, Vector3(0, -1, 0));
	this->model.rotateLocal(game->mouse_delta.y * 0.0005f, Vector3(-1, 0, 0));


	Vector3 f = this->model.frontVector().normalize();
	Vector3 p = this->model.position();
	Vector3 u = this->model.topVector().normalize();
	this->model.traslate(-f.x * speed_dt, -f.y * speed_dt, -f.z * speed_dt);


	Vector3 ideal_cam_pos = p + (f * 1.0f + u * 9.0f);
	Vector3 real_cam_pos = game->camera->eye.lerp(ideal_cam_pos, 0.1f);
	Vector3 cam_target = p - (f * 10.0f);

	game->camera->lookAt(real_cam_pos, cam_target, u);

	game->playerPosition = this->model.position();

	//Check if player has surpassed the aro

	if (currentAro < game->scene->listOfAros.size()) {

		GameObjectAro* aro = dynamic_cast<GameObjectAro*>(game->scene->listOfAros[currentAro]);

		if ((this->model.position().z > aro->model.position().z - this->current_speed / 10 && this->model.position().z < aro->model.position().z + this->current_speed / 10) && (this->model.position().x > aro->model.position().x - 10 * aro->scale && this->model.position().x < aro->model.position().x + 10 * aro->scale) && (this->model.position().y > aro->model.position().y - 10 * aro->scale && this->model.position().y < aro->model.position().y + 10 * aro->scale)) {

			aro->aro_surpassed = 1;
			currentAro++;

			if (currentAro == game->scene->listOfAros.size()) {

				GameObjectEnemy* enemy = dynamic_cast<GameObjectEnemy*>(game->scene->root->children[1]);

				if (enemy->currentAro < game->scene->listOfAros.size()) {
					game->gamestate = 3;
					game->is_winner = true;
				}
				else {
					game->gamestate = 3;
					game->is_winner = false;
				}
			}
		}
	}

	GameObject::update(elapsed_time);
}