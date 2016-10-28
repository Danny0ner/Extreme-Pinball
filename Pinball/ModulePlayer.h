#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	bool CreateLevers();

public:
	
	PhysBody* KickerL;
	PhysBody* KickerR;
	SDL_Texture* leftkicker;
	SDL_Texture* rightkicker;
	SDL_Texture* Spritesheeet;
	PhysBody* Spring;
	PhysBody* Pivot_spring;
	PhysBody* pivottex;

	bool presed = false;
	uint lever_fx;
	bool active_left = false;
	bool active_right = false;
};