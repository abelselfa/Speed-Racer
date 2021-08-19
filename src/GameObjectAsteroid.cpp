#include "GameObjectAsteroid.h"
#include "GameObjectEnemy.h"

void GameObjectAsteroid::update(float elapsed_time) {

    Game* game = Game::instance;

    float speed_dt = elapsed_time * current_speed;
    Vector3 to_target = orbitCenter - this->model.position();

    if ((sqrt(pow(to_target.x, 2) + pow(to_target.y, 2) + pow(to_target.z, 2))) != 0) {

        this->model.rotateAroundAPoint(speed_dt * DEG2RAD, rotationAxis, this->orbitCenter);
    }

    if ((sqrt(pow(this->model.position().x - game->playerPosition.x, 2) + pow(this->model.position().y - game->playerPosition.y, 2) + pow(this->model.position().z - game->playerPosition.z, 2))) < 100) {

        this->is_collider = true;
    }
    else {

        this->is_collider = false;
    }

    GameObject::update(elapsed_time);
}