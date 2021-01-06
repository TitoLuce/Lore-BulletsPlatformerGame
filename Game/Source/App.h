#ifndef __APP_H__
#define __APP_H__


#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "List.h"

#include "PugiXml/src/pugixml.hpp"
// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class EntityManager;
class GuiManager;
class ModuleFonts;

class Scene;
class Map;
class TitleScreen;
class Transition;
class LogoScreen;
//class Player;
class Collisions;
//class EnemyHandler;
class PathFinding;

class App
{
public:
	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	//Request to save & load
	void LoadRequest();
	void SaveRequest();

private:
	// Load config file
	pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	//Load & save on an xml file
	bool LoadGame();
	bool SaveGame();

public:
	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	EntityManager* entityManager;
	GuiManager* guiManager;
	ModuleFonts* fonts;

	Scene* scene;
	Map* map;
	TitleScreen* titleScreen;
	LogoScreen* logoScreen;
	//Player* player;
	Collisions* collisions;
	Transition* transition;
	//EnemyHandler* enemies;
	PathFinding* pathfinding;
	//Dont forget to declare new ones higher up -Luce TO: Luce, you doofus

private:
	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;
	uint frames;



	bool saveRequest, loadRequest;
	pugi::xml_document saveFile;
	pugi::xml_node save;
	

	// Frame variables
	PerfTimer ptimer;
	uint64 frameCount = 0;


	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;
	float cappedMs = -1;
	int cap = 0;
};

extern App* app;

#endif	// __APP_H__