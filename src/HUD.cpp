#include "HUD.h"
#include "game.h"

HUD::HUD(int sw, int sh) {

	Game* g = Game::instance;

	cam = new Camera();
	cam->setOrthographic(0, sw, 0, sh, 0.1, 2);
	cam->updateViewMatrix();

	minimap_back_mesh = new Mesh();
	minimap_back_mesh->vertices.resize(4);
	minimap_back_mesh->uvs.resize(4);

	minimap_back_mesh->vertices[0] = Vector3(sw - 256, 0, 1);
	minimap_back_mesh->vertices[1] = Vector3(sw, 0, 1);
	minimap_back_mesh->vertices[2] = Vector3(sw, 256, 1);
	minimap_back_mesh->vertices[3] = Vector3(sw - 256, 256, 1);

	minimap_back_mesh->uvs[0] = Vector2(0, 0);
	minimap_back_mesh->uvs[1] = Vector2(1, 0);
	minimap_back_mesh->uvs[2] = Vector2(1, 1);
	minimap_back_mesh->uvs[3] = Vector2(0, 1);

	minimap_back_mesh->indices.resize(6);
	minimap_back_mesh->indices[0] = 0;
	minimap_back_mesh->indices[1] = 1;
	minimap_back_mesh->indices[2] = 2;
	minimap_back_mesh->indices[3] = 0;
	minimap_back_mesh->indices[4] = 2;
	minimap_back_mesh->indices[5] = 3;

	minimap_back_mesh->genBuffers();

	//tex_shader = new Shader();
	tex_shader = g->resource_manager.getShader("HUD");
	//tex_shader->load("data/shaders/screen.vert", "data/shaders/screen.frag");
	//minimap_back_tex = new Texture();
	minimap_back_tex = g->resource_manager.getTexture("compass_256");
	minimap_back_tex_alpha = g->resource_manager.getTexture("compass_256_alpha");
}

void HUD::render() {
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	tex_shader->enable();
	tex_shader->setTexture("u_texture_diffuse", minimap_back_tex);
	tex_shader->setTexture("u_texture_alpha", minimap_back_tex_alpha);
	tex_shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	minimap_back_mesh->render(GL_TRIANGLES, tex_shader);
	tex_shader->disable();

	glEnable(GL_DEPTH_TEST);
}