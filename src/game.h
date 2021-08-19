/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)
	based on framework by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "ResourceManager.h"
#include "GameObjectMesh.h"
#include "Scene.h"

class Game
{
public:
	static Game* instance;

	ResourceManager resource_manager;

	int gamestate = 0;
	int menuOption = 0;
	double last_time = 0;
	bool can_press_key = 1;
	bool is_winner = false;
	bool anotherGame = false;

	Scene* scene;
	Vector3 playerPosition;
	Vector3 rivalPosition;
	Vector3 pivotPosition;
	
	bool cameraMode = false;

	//vector<GameObjectMesh*> objects;
	float angle = 0;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
    
    float time;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame
	bool mouse_locked; //tells if the mouse is locked (not seen)
	
	Camera* camera; //our global camera

	Game(SDL_Window* window);
	void init( void );
	void render( void );
	void update( double dt );
	void generateAros();
	void generateAsteroides();

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
    void onResize( SDL_Event e );
    
	void setWindowSize(int width, int height);
};


#endif 