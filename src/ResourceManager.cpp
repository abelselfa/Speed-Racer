#include "ResourceManager.h"

void ResourceManager::loadMesh(const string handle, const string path) {

	Mesh* mesh = new Mesh();
	mesh->loadASE(path.c_str()); //El c_str() convierte la string c++ a string c

	//añadir a mi diccionario
	m_meshes[handle] = mesh;
}

Mesh* ResourceManager::getMesh(const string handle) {

	//aqui, implementar algun tipo de aviso si el handle no existe en m_meshes

	return m_meshes.at(handle);
}

void ResourceManager::loadTexture(const string handle, const string path) {

	Texture* texture = new Texture();
	texture->load(path.c_str());

	//añadir a mi diccionario
	m_textures[handle] = texture;
}

Texture* ResourceManager::getTexture(const string handle) {

	//aqui, implementar algun tipo de aviso si el handle no existe en m_texture
	return m_textures.at(handle);
}

void ResourceManager::loadShader(const string handle,
	const string vspath,
	const string fspath) {
	Shader* shader = new Shader();

	if (!shader->load(vspath, fspath))
	{
		std::cout << "shader not found or error" << std::endl;
		exit(0);
	}

	//añadir a mi diccionario
	m_shaders[handle] = shader;
}

Shader* ResourceManager::getShader(const string handle) {

	//aqui, implementar algun tipo de aviso si el handle no existe en m_texture
	return m_shaders.at(handle);
}