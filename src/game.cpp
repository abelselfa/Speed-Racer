#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "GameObjectMesh.h"
#include "GameObjectEnemy.h"
#include "GameObjectPlayer.h"
#include "GameObjectAro.h"
#include "GameObjectSkybox.h"
#include "GameObjectAsteroid.h"
#include "GameObjectPivot.h"
#include "GameObjectArrow.h"
#include "HUD.h"
#include "Menu.h"

#include <cmath>

//some globals

//RenderToTexture* rt = NULL;

Game* Game::instance = NULL;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	mouse_locked = true;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
    
    //SDL_SetWindowSize(window, 50,50);

	//OpenGL flags
	//glEnable( GL_CULL_FACE ); //render one side of every triangle
	//glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0,0,500),Vector3(0,0,0), Vector3(0,1,0));
	camera->setPerspective(70.0f,(float)window_width/(float)window_height,0.1f,10000.0f); //set the projection, we want to be perspective
	
	//create a plane mesh
	/*        mesh = new Mesh();

	mesh->loadASE("data/assets/x3_fighter.ASE"); */
	//mesh->createPlane(10);   


	// LOADING SCREEN

	resource_manager.loadShader("menu", "data/shaders/menu.vert", "data/shaders/menu.frag");
	resource_manager.loadTexture("splash_screen", "data/assets/splash_screen.jpg");

	// RENDER SPLASH SCREEN

	resource_manager.loadTexture("loading_screen", "data/assets/loading_screen.tga");

	// RENDER LOADING SCREEN
	Menu* menu = new Menu(window_width, window_height);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	menu->renderLoading();
	SDL_GL_SwapWindow(this->window);


	//CARGAR RECURSOS

	resource_manager.loadTexture("menu_play", "data/assets/menu_play.tga");
	resource_manager.loadTexture("menu_options", "data/assets/menu_options.tga");
	resource_manager.loadTexture("menu_exit", "data/assets/menu_exit.tga");
	resource_manager.loadShader("objects", "data/shaders/simple.vert", "data/shaders/objects.frag");
	resource_manager.loadShader("player", "data/shaders/simple.vert", "data/shaders/player.frag");
	resource_manager.loadShader("skybox", "data/shaders/simple.vert", "data/shaders/skybox.frag");
	resource_manager.loadShader("HUD", "data/shaders/simple.vert", "data/shaders/HUD.frag");
	resource_manager.loadShader("aro", "data/shaders/simple.vert", "data/shaders/aro.frag");
	resource_manager.loadShader("arrow", "data/shaders/simple.vert", "data/shaders/arrow.frag");
	resource_manager.loadMesh("x3_fighter", "data/assets/x3_fighter.ASE");
	resource_manager.loadTexture("x3_fighter", "data/assets/x3_fighter.tga");
	resource_manager.loadMesh("sphere", "data/assets/sphere.ASE");
	resource_manager.loadTexture("milkyway", "data/assets/milkyway.tga");
	resource_manager.loadTexture("compass_256", "data/assets/compass_256.tga");
	resource_manager.loadTexture("compass_256_alpha", "data/assets/compass_256_alpha.tga");
	resource_manager.loadMesh("HUD_arrow", "data/assets/arrow.ASE");
	resource_manager.loadMesh("boxPivot", "data/assets/boxPivot.ASE");
	resource_manager.loadMesh("aro", "data/assets/aro.ASE");
	resource_manager.loadMesh("asteroide1", "data/assets/asteroide1.ASE");
	resource_manager.loadTexture("asteroide", "data/assets/asteroide.tga");
	resource_manager.loadTexture("victory", "data/assets/victory.tga");
	resource_manager.loadTexture("defeat", "data/assets/defeat.tga");

	//CREAR CONTENIDO DEL JUEGO
	
	GameObjectEnemy* x3_fighter = new GameObjectEnemy();
	x3_fighter->name = "X3_FIGHTER";
	x3_fighter->shader = resource_manager.getShader("objects");
	x3_fighter->mesh = resource_manager.getMesh("x3_fighter");
	x3_fighter->texture = resource_manager.getTexture("x3_fighter");
	x3_fighter->enemyType = GameObjectEnemy::ENEMY1;
	x3_fighter->model.setScale(1, 1, 1);
	x3_fighter->model.setTranslation(0, 0, 450);
	rivalPosition = x3_fighter->model.position();
	x3_fighter->base_speed = 80.0f;
	x3_fighter->current_speed = x3_fighter->base_speed;
	x3_fighter->is_collider = true;
	x3_fighter->setCollisionModel();
	x3_fighter->currentAro = 0;
	
	GameObjectPlayer* player = new GameObjectPlayer();
	player->name = "PLAYER";
	player->shader = resource_manager.getShader("player");
	player->mesh = resource_manager.getMesh("x3_fighter");
	player->texture = resource_manager.getTexture("x3_fighter");
	player->model.setScale(1, 1, 1);
	player->model.traslate(0, 0, 500);
	playerPosition = player->model.position();
	player->base_speed = 80.0f;
	player->current_speed = player->base_speed;
	player->is_collider = true;
	player->setCollisionModel();

	GameObjectSkybox* skybox = new GameObjectSkybox();
	skybox->name = "SKYBOX";
	skybox->shader = resource_manager.getShader("skybox");
	skybox->mesh = resource_manager.getMesh("sphere");
	skybox->texture = resource_manager.getTexture("milkyway");
	skybox->model.traslate(playerPosition.x, playerPosition.y, playerPosition.z);
	skybox->model.setScale(10, 10, 10);

	HUD* hud = new HUD(window_width, window_height);

	GameObjectArrow* arrow = new GameObjectArrow();
	arrow->name = "arrow";
	arrow->shader = resource_manager.getShader("arrow");
	arrow->mesh = resource_manager.getMesh("HUD_arrow");
	arrow->texture = new Texture();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//CREAR ESCENA

	scene = new Scene();
	
	scene->root->addChild(skybox);
	scene->root->addChild(x3_fighter);
	scene->root->addChild(player);
	scene->root->addChild(arrow);
	scene->hud = hud;
	scene->menu = menu;
	
	generateAros();
	generateAsteroides();

	gamestate = 1; // change to Menu Screen
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//*** Drawing code starts here ***

	switch (gamestate) {

	case 1: // Menu
		scene->menu->renderMenu(menuOption);
		break;

	case 2: // Gameplay
		//update view and projection matrices, and push gl matrices for fixed pipeline rendering
		camera->set();
		scene->root->render();
		scene->hud->render();
		break;

	case 3: // Winning / Losing
		scene->menu->renderFinalScreen();
		break;

	default:
		break;
	}

	//disable blending to draw text on top
	glDisable(GL_BLEND);

	//*** Drawing code ends here ***

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	double speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant
	SDL_ShowCursor(false); //Hacemos que no se vea el cursor

	if (long now = SDL_GetTicks() * 0.001f - last_time > 0.2f) {
		last_time = SDL_GetTicks() * 0.001f;
		can_press_key = 1;
	}

	switch (gamestate) {

	case 1: // Menu
		if ((keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) && menuOption > 0 && can_press_key) {
			menuOption--;
			can_press_key = 0;
		}
		if ((keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) && menuOption < 2 && can_press_key) {
			menuOption++;
			can_press_key = 0;
		}
		if (keystate[SDL_SCANCODE_RETURN]) {
			if (menuOption == 0) gamestate = 2;
			if (menuOption == 2) exit(0);
		}
		break;
	case 2: // Gameplay
		if (cameraMode) {
			//mouse input to rotate the cam
			if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
			{
				camera->rotate(mouse_delta.x * 0.005f, Vector3(0, -1, 0));
				camera->rotate(mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1, 0, 0)));

				if (mouse_locked)
				{
					int center_x = (int)floor(window_width * 0.5f);
					int center_y = (int)floor(window_height * 0.5f);
					//center_x = center_y = 50;
					SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
					//SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen

					this->mouse_position.x = (float)center_x;
					this->mouse_position.y = (float)center_y;
				}
			}

			//async input to move the camera around
			if (keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
			if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) camera->move(Vector3(0, 0, 1) * (float)speed);
			if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) camera->move(Vector3(0, 0, -1) * (float)speed);
			if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) camera->move(Vector3(1, 0, 0) * (float)speed);
			if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) camera->move(Vector3(-1, 0, 0) * (float)speed);

			for (int i = 0; i < scene->root->children.size(); i++) {

				if (GameObjectSkybox* skybox = dynamic_cast<GameObjectSkybox*>(scene->root->children[i])) {

					skybox->model.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
				}
			}
		}
		else {

			//to navigate with the mouse fixed in the middle

			if (mouse_locked)
			{
				int center_x = (int)floor(window_width * 0.5f);
				int center_y = (int)floor(window_height * 0.5f);
				//center_x = center_y = 50;
				SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
				//SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen

				this->mouse_position.x = (float)center_x;
				this->mouse_position.y = (float)center_y;
			}

			//angle += (float)seconds_elapsed * 10;

			//create empty list of colliders
			vector<GameObject*> colliders;

			scene->getColliders(colliders, scene->root);

			//test all collisions

			for (int i = 0; i < colliders.size() - 1; i++) {

				for (int j = (i + 1); j < colliders.size(); j++) {

					//test collisions
					GameObjectMesh* A = dynamic_cast<GameObjectMesh*>(colliders[i]);
					GameObjectMesh* B = dynamic_cast<GameObjectMesh*>(colliders[j]);

					A->collision_model->setTransform(A->model.m);
					B->collision_model->setTransform(B->model.m);

					bool collision = A->collision_model->collision(B->collision_model);

					if (collision == true) {

						if (GameObjectPlayer* player = dynamic_cast<GameObjectPlayer*>(A)) {

							if (GameObjectAsteroid* asteroid = dynamic_cast<GameObjectAsteroid*>(B)) {
								
								if (player->currentAro == 0) {
									player->model.setTranslation(0, 0, 500);
								}
								else {
									player->model.setTranslation(scene->listOfAros[player->currentAro - 1]->model.position().x, scene->listOfAros[player->currentAro - 1]->model.position().y, scene->listOfAros[player->currentAro - 1]->model.position().z);
								}
								camera->lookAt(Vector3(0, 0, 500), Vector3(0, 0, 0), Vector3(0, 1, 0));
							}
						}

						if (GameObjectPlayer* player = dynamic_cast<GameObjectPlayer*>(B)) {

							if (GameObjectAsteroid* asteroid = dynamic_cast<GameObjectAsteroid*>(A)) {
								
								if (player->currentAro == 0) {
									player->model.setTranslation(0, 0, 500);
								}
								else {
									player->model.setTranslation(scene->listOfAros[player->currentAro - 1]->model.position().x, scene->listOfAros[player->currentAro - 1]->model.position().y, scene->listOfAros[player->currentAro - 1]->model.position().z);
								}
								camera->lookAt(Vector3(0, 0, 500), Vector3(0, 0, 0), Vector3(0, 1, 0));
							}
						}
					}
				}
			}

			scene->root->update(seconds_elapsed);
		}
		break;
	case 3: // Menu
		if (keystate[SDL_SCANCODE_RETURN]) {

			anotherGame = true;
		}
		break;
	default:
		break;
	}
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
		case SDLK_c: cameraMode = !cameraMode;
	}
}

void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	/*if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}*/
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;

	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::generateAros() {

	scene->numberOfAros = rand() % 5 + 10; 
	int horDisplacement = 0;
	int vertDisplacement = 0;
	int distBetweenAros = rand() % 500 + 500; 

	GameObjectAro* aro;

	for (int i = 0; scene->listOfAros.size() < scene->numberOfAros; i++) {

		horDisplacement = rand() % 500 - 250; 
		vertDisplacement = rand() % 500 - 250; 

		aro = new GameObjectAro();
		aro->aro_surpassed = 0;
		aro->scale = 3;
		aro->name = "aro" + i;
		aro->position = i;
		aro->shader = resource_manager.getShader("aro");
		aro->mesh = resource_manager.getMesh("aro");
		aro->texture = new Texture();
		aro->model.rotateLocal(M_PI / 2, Vector3(1, 0, 0));
		aro->model.scale(aro->scale, aro->scale, aro->scale);

		if (i > 0) {

			Vector3 previousAroPos = scene->listOfAros[i - 1]->model.position();
			aro->model.traslate(previousAroPos.x + horDisplacement, previousAroPos.y + vertDisplacement, previousAroPos.z - distBetweenAros);
		}
		else {

			aro->model.traslate(0, 0, 0);
		}

		scene->listOfAros.push_back(aro);
	}

	for (int u = 0; u < scene->listOfAros.size(); u++) {

		scene->root->addChild(scene->listOfAros[u]);
	}
}

void Game::generateAsteroides() {
	
	int horDisplacement = 0;
	int vertDisplacement = 0;
	int zDisplacement = 0;
	float scale = 0;
	int tipoAsteroide = 0;
	GameObjectAsteroid* asteroid;

	for (int u = 0; u < scene->listOfAros.size(); u++) {

		int numberOfAsteroids = rand() % 30 + 20;

		for (int i = 0; i < numberOfAsteroids; i++) {

			if (u == 0) {
				//Do nothing
			}
			else {

				zDisplacement = rand() % 500 + (int)scene->listOfAros[u]->model.position().z;

				if (u > 0) {

					horDisplacement = rand() % 1000 - 500 + (int)scene->listOfAros[u - 1]->model.position().x; //Random horizontal displacement from the aro before in the range -25 to 25
					vertDisplacement = rand() % 1000 - 500 + (int)scene->listOfAros[u - 1]->model.position().y; //Random vertical displacement from the aro before in the range -25 to 25
				}

				asteroid = new GameObjectAsteroid();
				asteroid->name = "ASTEROID" + i;
				asteroid->shader = resource_manager.getShader("objects");

				asteroid->mesh = resource_manager.getMesh("asteroide1");
				asteroid->texture = resource_manager.getTexture("asteroide");
				scale = (rand() % 3 + 1) / 10;
				scale = ((0.2 - 0.05) * ((float)rand() / RAND_MAX)) + 0;
				asteroid->model.setScale(scale, scale, scale);
				asteroid->model.traslate(horDisplacement, vertDisplacement, zDisplacement);
				asteroid->orbitRadius = rand() % 1000;
				asteroid->orbitCenter = Vector3(asteroid->model.position().x - asteroid->orbitRadius, asteroid->model.position().y + asteroid->orbitRadius, asteroid->model.position().z - asteroid->orbitRadius);
				asteroid->rotationAxis = Vector3(((1 - 0) * ((float)rand() / RAND_MAX)) + 0, ((1 - 0) * ((float)rand() / RAND_MAX)) + 0, ((1 - 0) * ((float)rand() / RAND_MAX)) + 0);
				asteroid->base_speed = rand() % 80;
				asteroid->current_speed = asteroid->base_speed;
				asteroid->is_collider = false;
				asteroid->setCollisionModel();

				scene->root->addChild(asteroid);
			}
		}
	}
}

