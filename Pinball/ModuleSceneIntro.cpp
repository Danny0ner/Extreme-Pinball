#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/Bola2.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	pinballl = App->textures->Load("pinball/Pinball4.png");
	boings = App->textures->Load("pinball/Bola ilu2.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	Redlight = App->textures->Load("pinball/rojo2.png");
	grinding = App->audio->LoadFx("pinball/grind.wav");
	BouncerL = App->textures->Load("pinball/rebotador normal izquierdo2.png");
	BouncerR = App->textures->Load("pinball/rebotador normal derecho2.png");
	BouncerRA = App->textures->Load("pinball/rebotador activado derecho2.png");
	BouncerLA = App->textures->Load("pinball/rebotador activado izquierdo2.png");
	N10 = App->textures->Load("pinball/10 ilum.png");
	N25 = App->textures->Load("pinball/25 ilu.png");
	N50 = App->textures->Load("pinball/50 ilu.png");
	A = App->textures->Load("pinball/A-ilu.png");
	I = App->textures->Load("pinball/I-ilu.png");
	R = App->textures->Load("pinball/R-ilu.png");
	cono1 = App->textures->Load("pinball/cono arriba.png"); 
	cono2 = App->textures->Load("pinball/cono arriba medio.png");
	cono3 = App->textures->Load("pinball/cono abajo medio.png");
	cono4 = App->textures->Load("pinball/cono abajo.png");
	cono1tir = App->textures->Load("pinball/cono arriba tirado.png");
	cono2tir = App->textures->Load("pinball/cono arriba medio tirado.png");
	cono3tir = App->textures->Load("pinball/cono abajo medio tirado.png");
	cono4tir = App->textures->Load("pinball/cono abajo tirado.png");
	Box = App->textures->Load("pinball/caja.png");
	boxdestroy = App->audio->LoadFx("pinball/cajas.wav");
	clock = App->audio->LoadFx("pinball/rebotadores.wav");
	Startsound = App->audio->LoadFx("pinball/inicio fin.wav");
	airsound = App->audio->LoadFx("pinball/air.wav");
	greensound = App->audio->LoadFx("pinball/lucesverdes.wav");
	Greenlight1T = App->textures->Load("pinball/verde1.png");
	Greenlight2T = App->textures->Load("pinball/verde2.png");
	Greenlight3T = App->textures->Load("pinball/verde3.png");
	Greenlight4T = App->textures->Load("pinball/verde4.png");
	negroabajoiz = App->textures->Load("pinball/seta arriba izquierda normal.png");;
	negroarribader = App->textures->Load("pinball/seta derecha normal.png");;
	Negroarribaiz = App->textures->Load("pinball/seta abajo izquierda normal.png");
	setasalidaabajoiz = App->textures->Load("pinball/seta arriba izquierda.png");
	setasalidaarribaiz = App->textures->Load("pinball/seta abajo izquierda.png");
	setasalidader = App->textures->Load("pinball/seta derecha activada.png");
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	CreatePinball();
	electrify = App->audio->LoadFx("pinball/electrify.wav");
	G = App->textures->Load("pinball/G ilu.png");
	GR = App->textures->Load("pinball/R ilu red.png");
	GI = App->textures->Load("pinball/i ilu red.png");
	N = App->textures->Load("pinball/N ilu.png");
	D = App->textures->Load("pinball/D ilu.png");
	X2 = App->textures->Load("pinball/x2.png");
	X2ball = App->textures->Load("pinball/bola azul.png");
	dude = App->audio->LoadFx("pinball/dude.wav");
	Spritesheet = App->textures->Load("pinball/Spritesheet.png");
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	sprintf_s(title, "Extreme Pinball Score: %06d Balls left &d", points, lastpoints, balls);
	App->window->SetTitle(title);

	App->renderer->Blit(pinballl, 2, 0, NULL, 1.0f);
	if (start == false) {
		lastpoints = points;
		points = 0;
		App->audio->PlayFx(Startsound);
		circles.add(App->physics->CreateCircle(450, 665, 8, true));
		circles.getLast()->data->listener = this;
		start = true;
		balls--;
		AIR = 0;
		cone1down = false;
		cone2down = false;
		cone3down = false;
		cone4down = false;
		rightSavior = false;
		leftSavior = false;
		box1down = false;
		box2down = false;
		box3down = false;
		x2 = false;
		electrifed = false;
	}
	if (cone1down == true && cone2down == true && cone3down == true && cone4down == true && electrifed == false) {
		App->audio->PlayFx(electrify);
		x2 = true;
		electrifed = true;
	}
	if (points - prepoints == 25 ) {
		pointstime25 = SDL_GetTicks();
	}
	if (SDL_GetTicks() <= pointstime25 + 300) {
		App->renderer->Blit(N25, 323, 602, NULL, 1.0f);
		App->renderer->Blit(N25, 123, 602, NULL, 1.0f);
	}
	if (points - prepoints == 50) {
		pointstime50 = SDL_GetTicks();
	}
	if (SDL_GetTicks() <= pointstime50 + 300) {
		App->renderer->Blit(N50, 89, 573, NULL, 1.0f);
		App->renderer->Blit(N50, 355, 573, NULL, 1.0f);
	}
	if (points-prepoints == 10) {
		pointstime10 = SDL_GetTicks();
	}
	
	if (SDL_GetTicks() <= pointstime10) {
		App->renderer->Blit(N10, 165, 638, NULL, 1.0f);
		
		if (points-points10 >= 20) App->renderer->Blit(N10, 286, 638, NULL, 1.0f);
	}
	if (SDL_GetTicks() > pointstime10) points10 = points;
	prepoints = points;
	if (greenlight1 == true) {
		greenlight1timer = SDL_GetTicks();
		greenlight1 = false;
	}
	if (SDL_GetTicks() <= greenlight1timer + 100) {
		App->renderer->Blit(Greenlight1T, 149, 91, NULL, 1.0f);
		
	}
	if (greenlight2 == true) {
		greenlight2timer = SDL_GetTicks();
		greenlight2 = false;
	}
	if (SDL_GetTicks() <= greenlight2timer + 100) {
		App->renderer->Blit(Greenlight2T, 169, 67, NULL, 1.0f);
		
	}
	if (greenlight3 == true) {
		greenlight3timer = SDL_GetTicks();
		greenlight3 = false;
	}

	if (SDL_GetTicks() <= greenlight3timer + 100) {
		App->renderer->Blit(Greenlight3T, 197, 52, NULL, 1.0f);
		
	}
	if (greenlight4 == true) {
		greenlight4timer = SDL_GetTicks();
		greenlight4 = false;
	}
	if (SDL_GetTicks() <= greenlight4timer + 100) {
		App->renderer->Blit(Greenlight4T, 233, 37, NULL, 1.0f);
	}

	if (bouncerr == false)
		App->renderer->Blit(BouncerR, 320, 635, NULL, 1.0f);
	if (bouncerr == true)
		bouncerrtimer = SDL_GetTicks();
	if (SDL_GetTicks() <= bouncerrtimer+100) {
		App->renderer->Blit(BouncerRA, 320, 635, NULL, 1.0f);
		bouncerr = false;
	}
	if (bouncerl == false)
		App->renderer->Blit(BouncerL, 115, 635, NULL, 1.0f);

	if (bouncerl == true) {
		bouncerltimer = SDL_GetTicks();
	}
	if (SDL_GetTicks() <= bouncerltimer + 100) {
		App->renderer->Blit(BouncerLA, 115, 635, NULL, 1.0f);
		bouncerl = false;
	}

	
	if (squares1 == true) {
		squares1timer = SDL_GetTicks();
		squares1 = false;
	}
	if (SDL_GetTicks() <= (squares1timer + 1000)) {
		App->renderer->Blit(Redlight, 270, 134, NULL, 1.0f);
	// 20 i 17
		App->renderer->Blit(Redlight, 253, 92, NULL, 1.0f);
		if (SDL_GetTicks() > timerredlights +200) points += 25;
		timerredlights = SDL_GetTicks();
	}
	if (squares2 == true) {
		squares2timer = SDL_GetTicks();
		squares2 = false;
	}
	if (SDL_GetTicks() <= (squares2timer + 1000)) {
		App->renderer->Blit(Redlight, 200, 92, NULL, 1.0f);
		App->renderer->Blit(Redlight, 221, 135, NULL, 1.0f);
		if (SDL_GetTicks() > timerredlights2 +200) points += 25;
		timerredlights2 = SDL_GetTicks();
	}
	if (bouncer1 == true) {
		bouncer1timer = SDL_GetTicks();
		bouncer1 = false;
	}
	if (SDL_GetTicks() <= (bouncer1timer + 100)) {
		App->renderer->Blit(boings, 253, 195, NULL, 1.0f);
		pointstime10 += 50;
	}
	if (bouncer2 == true) {
		bouncer2timer = SDL_GetTicks();
		bouncer2 = false;
	}
	if (SDL_GetTicks() <= (bouncer2timer + 100)) {
		App->renderer->Blit(boings, 189, 272, NULL, 1.0f);
		pointstime10 += 50;
	}
	if (bouncer3 == true) {
		bouncer3timer = SDL_GetTicks();
		bouncer3 = false;
	}
	if (SDL_GetTicks() <= (bouncer3timer + 100)) {
		App->renderer->Blit(boings, 308, 275, NULL, 1.0f);
		pointstime10 += 50;
	}

	if (tunnel == true)
	{
		tunneltime = SDL_GetTicks();
		tunnel = false;
	}
	if (SDL_GetTicks() >= tunnel + 1000) avaibletunnel = true;
	if( Grind == 1){
		App->renderer->Blit(G, 303, 90, NULL, 1.0f);
	
	}
	if (Grind == 2) {
		App->renderer->Blit(G, 303, 90, NULL, 1.0f);
		App->renderer->Blit(GR, 315, 109, NULL, 1.0f);
		
	}
	if (Grind == 3) {
		App->renderer->Blit(G, 303, 90, NULL, 1.0f);
		App->renderer->Blit(GR, 315, 109, NULL, 1.0f);
		App->renderer->Blit(GI, 333, 128, NULL, 1.0f);
		
	}
	if (Grind == 4) {
		App->renderer->Blit(G, 303, 90, NULL, 1.0f);
		App->renderer->Blit(GR, 315, 109, NULL, 1.0f);
		App->renderer->Blit(GI, 333, 128, NULL, 1.0f);
		App->renderer->Blit(N, 335, 145, NULL, 1.0f);
		
	}
	if (Grind == 5) {
		App->renderer->Blit(G, 303, 90, NULL, 1.0f);
		App->renderer->Blit(GR, 315, 109, NULL, 1.0f);
		App->renderer->Blit(GI, 333, 128, NULL, 1.0f);
		App->renderer->Blit(N, 335, 145, NULL, 1.0f);
		App->renderer->Blit(D, 343, 175, NULL, 1.0f);
	}
	if (Grind > 5) {
		Grind = 0;
	}
	if (AIR == 1)
	{
		App->renderer->Blit(A, 97, 538, NULL, 1.0f);
		
	}
	if (AIR == 2) {
		App->renderer->Blit(I, 105, 518, NULL, 1.0f);
		App->renderer->Blit(A, 97, 538, NULL, 1.0f);
	}
	if (AIR == 3) {
		App->renderer->Blit(A, 97, 538, NULL, 1.0f);
		App->renderer->Blit(I, 105, 518, NULL, 1.0f);
		App->renderer->Blit(R, 118, 499, NULL, 1.0f);
	}
	if (AIR > 3) AIR = 0;
	if (cone1down == false) {
		App->renderer->Blit(cono1, 400, 322, NULL, 1.0f);
	}
	if (box1down == false) {
		App->renderer->Blit(Box, 40, 180, NULL, 1.0f);
	}
	if (box2down == false) {
		App->renderer->Blit(Box, 48, 200, NULL, 1.0f);
	}
	if (box3down == false) {
		App->renderer->Blit(Box, 32, 160, NULL, 1.0f);
	}
	if (cone2down == false){
		App->renderer->Blit(cono2, 415, 360, NULL, 1.0f);
		
	}
	if (cone3down == false) {
		App->renderer->Blit(cono3, 415, 405, NULL, 1.0f);
	}
	if (cone4down == false) {
		App->renderer->Blit(cono4, 400, 433, NULL, 1.0f);
	}
	if (cone1down == true) {
		App->renderer->Blit(cono1tir, 400, 322, NULL, 1.0f);
	}
	if (cone2down == true) {
		App->renderer->Blit(cono2tir, 415, 360, NULL, 1.0f);

	}
	if (cone3down == true) {
		App->renderer->Blit(cono3tir, 415, 405, NULL, 1.0f);
	}
	if (cone4down == true) {
		App->renderer->Blit(cono4tir, 400, 433, NULL, 1.0f);
	}
	if (setaarribader == false) {
		App->renderer->Blit(negroarribader, 400, 277, NULL, 1.0f);
	}
	if (SDL_GetTicks() <= setaarribadertime + 100) {
		App->renderer->Blit(setasalidader, 376, 277, NULL, 1.0f);
	}
	if (SDL_GetTicks() > setaarribadertime + 100) {
		setaarribader = false;
	}
	if (setaabajoiz == false) {
		App->renderer->Blit(negroabajoiz, 77, 400, NULL, 1.0f);
	}
	if (SDL_GetTicks() <= setaabajoiztime + 100) {
		App->renderer->Blit(setasalidaabajoiz, 79, 390, NULL, 1.0f);
	}
	if (SDL_GetTicks() > setaabajoiztime + 100) {
		setaabajoiz = false;
	}
	if (setaabajoder == false) {
		App->renderer->Blit(negroarribader, 386, 488, NULL, 1.0f);
	}
	if (SDL_GetTicks() <= setaabajodertime + 100) {
		App->renderer->Blit(setasalidader, 360, 488, NULL, 1.0f);
	}
	if (SDL_GetTicks() > setaabajodertime + 100) {
		setaabajoder = false;
	}
	if (setaarribaiz == false) {
		App->renderer->Blit(Negroarribaiz, 98, 150, NULL, 1.0f);
	}
	if (SDL_GetTicks() <= setaarribaiztime + 100) {
		App->renderer->Blit(setasalidaarribaiz, 104, 158, NULL, 1.0f);
	}
	if (SDL_GetTicks() > setaarribaiztime + 100) {
		setaarribaiz = false;
	}
	if (x2 == true) {
		App->renderer->Blit(X2, 239, 477, NULL, 1.0f);
	}
	if (SDL_GetTicks() < dudetimer + 1000) {
		//App->renderer->Blit(Spritesheet, 239, 477, NULL, 1.0f);
	}
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 8, true));
		circles.getLast()->data->listener = this;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 3,3,0, true));
		boxes.getLast()->data->listener = this;
	}
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {
		balls = 5;
	}
	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64, 0));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(x2 == false)
			App->renderer->Blit(circle, x, y, NULL, 1.0f,0);
		if (x2 == true) {
			App->renderer->Blit(X2ball, x, y, NULL, 1.0f, 0);
		}
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
	
	p2List_item<PhysBody*>* iterator = Pinballl.getFirst()->next;

	if (bodyB == iterator->data)
	{
		bouncer1 = true;
		App->audio->PlayFx(bonus_fx); points += 10;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		bouncer2 = true;
		App->audio->PlayFx(bonus_fx); points += 10;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		bouncer3 = true;
		App->audio->PlayFx(bonus_fx); points += 10;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		squares1 = true;
	}
	iterator = iterator->next;

	if (bodyB == iterator->data) {
		squares2 = true;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(grinding);
		Grind++;
		points += 50;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		bouncerl = true;
		points += 10;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		bouncerr = true;
		points += 10;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(clock);
		setaarribaiz = true;
		setaarribaiztime = SDL_GetTicks();
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(clock);
		setaabajoiz = true;
		setaabajoiztime = SDL_GetTicks();
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(clock);
		setaarribader = true;
		setaarribadertime = SDL_GetTicks();
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(clock);
		setaabajoder = true;
		setaabajodertime = SDL_GetTicks();
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(greensound);
		greenlight1 = true;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(greensound);
		greenlight2 = true;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(greensound);
		greenlight3 = true;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		greenlight4 = true;
		App->audio->PlayFx(greensound);
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (rightSavior == false) {
			bodyA->body->ApplyForceToCenter(b2Vec2(0.0f, -150.0f), true);
			rightSavior = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (leftSavior == false) {
			bodyA->body->ApplyForceToCenter(b2Vec2(0.0f, -150.0f), true);
			leftSavior = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (balls > 0) {
			start = false;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
	
		App->audio->PlayFx(airsound);
		points += 25;
		AIR++;
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (cone1down == false) {
			App->physics->HorizontalRebound(bodyA);
			cone1down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		
		if (cone1down == false) {
			App->physics->VerticalRebound(bodyA);
			cone1down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (cone2down == false) {
			App->physics->HorizontalRebound(bodyA);
			cone2down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		
		if (cone2down == false) {
			App->physics->VerticalRebound(bodyA);
			cone2down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (cone3down == false) {
			App->physics->HorizontalRebound(bodyA);
			cone3down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (cone3down == false) {
			App->physics->VerticalRebound(bodyA);
			cone3down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (cone4down == false) {
			App->physics->HorizontalRebound(bodyA);
			cone4down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (cone4down == false) {
			App->physics->VerticalRebound(bodyA);
			cone4down = true;
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (box1down == false) {
			App->physics->HorizontalRebound(bodyA);
			box1down = true;
			App->audio->PlayFx(boxdestroy);
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {

		if (box1down == false) {
			App->physics->VerticalRebound(bodyA);
			box1down = true;
			App->audio->PlayFx(boxdestroy);
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (box2down == false) {
			App->physics->HorizontalRebound(bodyA);
			box2down = true;
			App->audio->PlayFx(boxdestroy);
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (box2down == false) {
			App->physics->VerticalRebound(bodyA);
			box2down = true;
			App->audio->PlayFx(boxdestroy);
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (box3down == false) {
			App->physics->HorizontalRebound(bodyA);
			box3down = true;
			App->audio->PlayFx(boxdestroy);
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		if (box3down == false) {
			App->physics->VerticalRebound(bodyA);
			box3down = true;
			App->audio->PlayFx(boxdestroy);
		}
	}
	iterator = iterator->next;
	if (bodyB == iterator->data) {
		App->audio->PlayFx(dude);
		duude = true;
		dudetimer = SDL_GetTicks();
	}
	/*if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
void ModuleSceneIntro::CreatePinball() {

	int LeftBar[14] = {
		82, 628,
		89, 628,
		89, 709,
		167, 753,
		152, 753,
		82, 713,
		82, 628
	};

	int Pinball[174] = {
		506, 834,
		360, 836,
		316, 817,
		317, 798,
		419, 747,
		418, 572,
		402, 552,
		402, 473,
		442, 452,
		442, 333,
		410, 310,
		410, 282,
		442, 265,
		442, 195,
		388, 190,
		381, 161,
		370, 134,
		357, 117,
		342, 101,
		321, 93,
		322, 81,
		337, 63,
		358, 74,
		380, 88,
		397, 102,
		418, 123,
		437, 153,
		444, 185,
		447, 199,
		447, 764,
		471, 764,
		471, 190,
		468, 171,
		452, 136,
		423, 99,
		386, 70,
		353, 47,
		322, 35,
		291, 31,
		257, 31,
		223, 36,
		196, 47,
		166, 65,
		146, 84,
		127, 103,
		120, 115,
		121, 126,
		131, 156,
		110, 168,
		82, 181,
		71, 184,
		61, 152,
		46, 99,
		24, 103,
		31, 192,
		38, 252,
		44, 287,
		55, 335,
		67, 376,
		84, 411,
		101, 445,
		120, 471,
		125, 478,
		123, 486,
		119, 490,
		85, 551,
		81, 563,
		64, 605,
		63, 744,
		174, 804,
		175, 813,
		155, 835,
		20, 835,
		20, 13,
		507, 13,
		507, 821,
		507, 821,
		507, 821,
		175, 813,
		175, 813,
		155, 835,
		20, 835,
		20, 13,
		507, 13,
		507, 821,
		507, 821,
		507, 821
	};


	// Pivot 0, 0
	int rightBar[14] = {
		390, 628,
		397, 628,
		398, 712,
		329, 754,
		314, 752,
		390, 709,
		390, 629
	};

	// Pivot 0, 0
	int topleftbox[10] = {
		174, 101,
		191, 94,
		211, 134,
		193, 143,
		174, 101
	};

	// Pivot 0, 0
	int topmidbox[10] = {
		223, 102,
		239, 93,
		259, 134,
		244, 142,
		224, 103
	};


	// Pivot 0, 0
	int toprightbox[10] = {
		271, 101,
		287, 93,
		308, 134,
		291, 143,
		271, 101
	};
	int RightBod[38] = {
		324, 722,
		329, 722,
		340, 716,
		349, 712,
		357, 707,
		363, 703,
		368, 696,
		367, 682,
		367, 670,
		367, 658,
		367, 646,
		367, 637,
		362, 636,
		361, 640,
		356, 654,
		350, 670,
		343, 686,
		334, 704,
		325, 718
	};
	int RightReb[14] = {
		324, 723,
		322, 719,
		323, 713,
		357, 637,
		360, 634,
		363, 634,
		325, 721
	};

	int LeftBod[16] = {
		123, 638,
		119, 634,
		115, 636,
		149, 722,
		153, 723,
		157, 721,
		157, 716,
		124, 640
	};
	int LeftReb[14] = {
		115, 639,
		115, 697,
		115, 701,
		118, 703,
		148, 721,
		152, 722,
		117, 642
	};
	int Bouncer1[14] = {
		394, 494,
		390, 498,
		389, 504,
		389, 511,
		390, 516,
		394, 521,
		394, 497
	};
	int Bouncer2[14] = {
		80, 407,
		93, 434,
		97, 430,
		97, 421,
		93, 413,
		89, 407,
		83, 406
	};
	int Bouncer3[16] = {
		128, 156,
		129, 162,
		125, 167,
		119, 171,
		111, 173,
		105, 173,
		101, 169,
		128, 156
	};
	int Air[10] = {
		85, 546,
		88, 557,
		125, 487,
		114, 488,
		85, 542
	};
	Pinballl.add(App->physics->CreateChain((SCREEN_WIDTH / 4.5) - 117, 0, Pinball, 154, 0.2));
	Pinballl.add(App->physics->CreateStaticCircle(506 - 229, 219, 27.5, 1.5));
	Pinballl.add(App->physics->CreateStaticCircle(440 - 228, 297, 27.5, 1.5));
	Pinballl.add(App->physics->CreateStaticCircle(560 - 228, 300, 27.5, 1.5));
	Pinballl.add(App->physics->CreateRectangleSensor((SCREEN_WIDTH / 2) + 9, 120, 20, 10));
	Pinballl.add(App->physics->CreateRectangleSensor((SCREEN_WIDTH / 2) - 42, 120, 20, 10));
	Pinballl.add(App->physics->CreateRectangleSensor((SCREEN_WIDTH / 2) + 170, 210, 10, 30));
	Pinballl.add(App->physics->CreateChain(0, 0, LeftBod, 16, 1.5));
	Pinballl.add(App->physics->CreateChain(0, 0, RightReb, 14, 1.5));
	Pinballl.add(App->physics->CreateChain(0, 0, Bouncer3, 15, 2));
	Pinballl.add(App->physics->CreateChain(0, 0, Bouncer2, 14, 2));
	Pinballl.add(App->physics->CreateChain( 15, -214, Bouncer1, 14, 2));
	Pinballl.add(App->physics->CreateChain(0, 0, Bouncer1, 14, 2));
	Pinballl.add(App->physics->CreateRectangleSensor((SCREEN_WIDTH / 2) - 105, 99, 10, 10));
	Pinballl.add(App->physics->CreateRectangleSensor((SCREEN_WIDTH / 2) - 83, 77, 12, 12));
	Pinballl.add(App->physics->CreateRectangleSensor((SCREEN_WIDTH / 2) - 53, 63, 14, 14));
	Pinballl.add(App->physics->CreateRectangleSensor((SCREEN_WIDTH / 2)-16, 53, 18, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(410, 745, 10, 10));
	Pinballl.add(App->physics->CreateRectangleSensor(70, 745, 10, 10));
	Pinballl.add(App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50));
	Pinballl.add(App->physics->CreateSensorChain(0, 0, Air, 10, 0));
	//cones
	Pinballl.add(App->physics->CreateRectangleSensor(416, 336, 21, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(417, 335, 18, 21));
	Pinballl.add(App->physics->CreateRectangleSensor(428, 374, 21, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(429, 373, 18, 21));
	Pinballl.add(App->physics->CreateRectangleSensor(428, 416, 21, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(429, 415, 18, 21));
	Pinballl.add(App->physics->CreateRectangleSensor(416, 451, 21, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(417, 450, 18, 21));
	//Boxes

	Pinballl.add(App->physics->CreateRectangleSensor(55, 200, 21, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(56, 199, 18, 21));
	Pinballl.add(App->physics->CreateRectangleSensor(64, 220, 21, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(65, 219, 18, 21));
	Pinballl.add(App->physics->CreateRectangleSensor(46, 180, 21, 18));
	Pinballl.add(App->physics->CreateRectangleSensor(47, 179, 18, 21));
	
	


	Pinballl.add(App->physics->CreateRectangle(40, 120, 10, 10, 0.2, false));

	Pinballl.add(App->physics->CreateChain(0, 0, RightBod, 38, 0));
	Pinballl.add(App->physics->CreateChain(0, 0, LeftReb, 14, 0));

	Pinballl.add(App->physics->CreateChain(0, 1, LeftBar, 13, 0.5));
	Pinballl.add(App->physics->CreateChain(0, 1, rightBar, 13, 0.5));
	//Pinballl.add(App->physics->CreateChain(0, 0, Bouncer1, 14, 2));

	Pinballl.add(App->physics->CreateChain(0, 1, topleftbox, 9, 0.5));
	Pinballl.add(App->physics->CreateChain(0, 1, topmidbox, 9, 0.5));
	Pinballl.add(App->physics->CreateChain(0, 1, toprightbox, 9, 0.5));
}