#include "GameObjectEnemy.h"


void GameObjectEnemy::update(float elapsed_time) {

    Game* game = Game::instance;

    //Code to move to the next aro

    float speed_dt = elapsed_time * (current_speed + rand() % 20 + 60);

    if (currentAro < game->scene->numberOfAros) {

        Vector3 to_target = game->scene->listOfAros[currentAro]->model.position() - this->model.position();

        if (this->model.position().z <= game->scene->listOfAros[currentAro]->model.position().z) {

            currentAro++;
        }

        float angle = acos(to_target.normalize().dot(this->model.frontVector().normalize()));
        Vector3 axis = to_target.normalize().cross(this->model.frontVector().normalize());
        Matrix44 inv = this->model;
        inv.inverse();

        Vector3 rot_axis = inv.rotateVector(axis);

        if ((sqrt(pow(rot_axis.x, 2) + pow(rot_axis.y, 2) + pow(rot_axis.z, 2))) != 0) {

            this->model.rotateLocal(-0.005, rot_axis.normalize());
        }

        Vector3 f = this->model.frontVector().normalize();
        this->model.traslate(-f.x * speed_dt, -f.y * speed_dt, -f.z * speed_dt);
    }

    game->rivalPosition = this->model.position();

    GameObject::update(elapsed_time);
}

/*void GameObjectEnemy::dodgeAsteroid(GameObjectAsteroid asteroid) {

    if (asteroid.orbitRadius <= 5) {
        this->model.rotateLocal(-0.005, Vector3(0,1,0));
    }
    else {
        this->model.rotateLocal(-0.005, asteroid.rotationAxis);
        cout << "esquiva";
    }
}*/