#include "Scene.h"

Scene::Scene() {
    this->root = new GameObject();
    this->numberOfAros = 0;
}

Scene::~Scene() {
    delete this->root;
}

void Scene::getColliders(std::vector<GameObject*>& colliders, GameObject* current) {

    if (current->is_collider) {

        colliders.push_back(current);
    }

    for (int i = 0; i < current->children.size(); i++) {

        getColliders(colliders, current->children[i]);
    }
}