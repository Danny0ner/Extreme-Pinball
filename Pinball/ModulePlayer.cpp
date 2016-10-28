#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	leftkicker = App->textures->Load("pinball/PalaIzquierda.png");
	rightkicker = App->textures->Load("pinball/PalaDerecha.png");
	KickerL = App->physics->CreatePaddleL(169, 762, (-5 * DEGTORAD), -60 * DEGTORAD);
	KickerR = App->physics->CreatePaddleR(312, 762, (180* DEGTORAD), 120 * DEGTORAD);
	Spring = App->physics->CreateRectangle(458, 650, 20, 10, 0,true);
	Pivot_spring = App->physics->CreateRectangle(458, 755, 20, 10,0, false);
	App->physics->CreateDistanceJoint(Spring, Pivot_spring);
	Spritesheeet = App->textures->Load("pinball/Spritesheet.png");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		App->physics->LKickerMove();
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		App->physics->LKickerStop();

	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		App->physics->RKickerMove();
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		App->physics->RKickerStop();

	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		Spring->body->ApplyForceToCenter(b2Vec2(0.0f, 0.000000000000000000000000000000000000001f), true);
	}
	else
	{
		Spring->body->ApplyForceToCenter(b2Vec2(0.0f, -70.0f), true);
	}
	SDL_Rect rect;
	int r, u;
	Spring->GetPosition(r, u);
	iPoint springpos((r), (u));
	rect.x = 517; rect.y = 447; rect.w = 26; rect.h = 59;
	App->renderer->Blit(Spritesheeet, springpos.x, springpos.y, &rect, 1.0f);
	
	rect.x = 345; rect.y = 863; rect.w = 97; rect.h = 87;
	App->renderer->Blit(Spritesheeet, 440, 760, &rect, 1.0f);
	int x, y;
	KickerL->GetPosition(x, y);
	iPoint LeftKick((x), (y));
	App->renderer->Blit(leftkicker, LeftKick.x , LeftKick.y, NULL, 1.0f, KickerL->GetRotation(), 0,0);


	int z, t;
	KickerR->GetPosition(z, t);
	iPoint RightKick(PIXEL_TO_METERS(z), PIXEL_TO_METERS(t));
	App->renderer->Blit(rightkicker, RightKick.x+260, RightKick.y + 740, NULL, 1.0f, KickerR->GetRotation(), 50,0);

	return UPDATE_CONTINUE;

}





