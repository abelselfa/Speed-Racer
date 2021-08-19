#pragma once
#include <unordered_map>
#include <string>
#include "mesh.h"
#include "texture.h"
#include "shader.h"

using namespace std;

class ResourceManager {
private:
	unordered_map<string, Mesh*> m_meshes;
	unordered_map<string, Texture*> m_textures;
	unordered_map<string, Shader*> m_shaders;

public:
	void loadMesh(const string handle, const string path);

	Mesh* getMesh(const string handle);

	void loadTexture(const string handle, const string path);

	Texture* getTexture(const string handle);

	void loadShader(const string handle, const string vspath, const string fspath);

	Shader* getShader(const string handle);
};