#include "GameObjectArrow.h"
#include "GameObjectPlayer.h"

void GameObjectArrow::update(float elapsed_time) {

	Game* game = Game::instance;

	if (GameObjectPlayer* player = dynamic_cast<GameObjectPlayer*>(game->scene->root->children[2])) {
        
        if (player->currentAro < game->scene->numberOfAros) {
            
            Vector3 to_target = (game->scene->listOfAros[player->currentAro]->model.position() - this->model.position()).normalize();

            float angle = acos(to_target.normalize().dot(this->model.frontVector().normalize()));
            Vector3 axis = to_target.normalize().cross(this->model.frontVector().normalize());
            Matrix44 inv = this->model;
            inv.inverse();

            Vector3 rot_axis = inv.rotateVector(axis);
            
            if ((sqrt(pow(rot_axis.x, 2) + pow(rot_axis.y, 2) + pow(rot_axis.z, 2))) != 0) {

                this->model.setTranslation(player->model.position().x + 6 * player->model.topVector().x, player->model.position().y + 6 * player->model.topVector().y, player->model.position().z + 6 * player->model.topVector().z);
                //this->model.rotateAroundAPoint(player->model.position());
                
                this->model.m[8] = -to_target.x;
                this->model.m[9] = -to_target.y;
                this->model.m[10] = -to_target.z;
            }
        }
	}
	
	GameObject::update(elapsed_time);
}