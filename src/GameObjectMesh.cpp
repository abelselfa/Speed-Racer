#include "GameObjectMesh.h"
#include "GameObjectAro.h"
#include "GameObjectSkybox.h"
#include "GameObjectPlayer.h"
#include "GameObjectArrow.h"

void GameObjectMesh::render() {
    Game* g = Game::instance;

    //create model matrix for our plane mesh

    shader->enable();

    glEnable(GL_DEPTH_TEST);

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

    if (GameObjectArrow* arrow = dynamic_cast<GameObjectArrow*>(this)) {

        if(GameObjectPlayer * player = dynamic_cast<GameObjectPlayer*>(g->scene->root->children[2])) {
            Vector3 to_target = arrow->model.position() - g->scene->listOfAros[player->currentAro]->model.position();
            float distance = (sqrt(pow(to_target.x, 2) + pow(to_target.y, 2) + pow(to_target.z, 2)));
            shader->setUniform1("u_distance", distance/1000);
        }
    }

    if (GameObjectAro* aro = dynamic_cast<GameObjectAro*>(this)) {

        shader->setUniform1("u_aro_surpassed", aro->aro_surpassed);
        shader->setUniform1("u_position", aro->position);

        int meta = g->scene->listOfAros.size();
        shader->setUniform1("u_meta", meta);

        if (GameObjectPlayer* player = dynamic_cast<GameObjectPlayer*>(g->scene->root->children[2])) {
            shader->setUniform1("u_next_aro", player->currentAro);
        }
    }

    //resource_manager.getMesh("x3_fighter")->render(GL_TRIANGLES, shader);
    this->mesh->render(GL_TRIANGLES, this->shader);

    //disable the shader
    shader->disable();

    GameObject::render();
}

void GameObjectMesh::setCollisionModel() {

	collision_model = newCollisionModel3D();

	collision_model->setTriangleNumber(this->mesh->indices.size() / 3);

	for (int i = 0; i < mesh->indices.size() / 3; i++) {

		Vector3 v1 = mesh->vertices[mesh->indices[i * 3]];
		Vector3 v2 = mesh->vertices[mesh->indices[i * 3 + 1]];
		Vector3 v3 = mesh->vertices[mesh->indices[i * 3 + 2]];

		collision_model->addTriangle(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
	}

	collision_model->finalize();
}