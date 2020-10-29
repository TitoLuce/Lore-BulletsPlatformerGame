//#include "DeathScreen.h"
//
//#include "App.h"
//#include "Window.h"
//#include "Textures.h"
//#include "Render.h"
//#include "Audio.h"
//#include "Transition.h"
//#include "Scene.h"
//#include "Input.h"
//
//#include "SDL/include/SDL_scancode.h"
//
//DeathScreen::DeathScreen() : Module() {
//	name.Create("logoScreen");
//}
//
//DeathScreen::~DeathScreen() {}
//
//// Load assets
//bool DeathScreen::Start() {
//	
//	deathScreenTexture = app->tex->Load("Assets/DeathScreen.png");
//	return true;
//}
//
//bool DeathScreen::PreUpdate()
//{
//	return true;
//}
//
//bool DeathScreen::Update(float dt) {
//	return true;
//}
//
//// Update: draw background
//bool DeathScreen::PostUpdate() {
//	// Draw everything --------------------------------------
//	app->render->DrawTexture(deathScreenTexture, 0, 0, false);
//	return true;
//}
//
//bool DeathScreen::CleanUp() {
//	app->tex->UnLoad(deathScreenTexture);
//
//	return true;
//}
//
//
//void DeathScreen::Init()
//{
//	active = false;
//}