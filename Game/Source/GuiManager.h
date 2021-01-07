#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	GuiManager();

	// Destructor
	virtual ~GuiManager();

	void init();

	// Called before render is available
	//bool Awake(pugi::xml_node&);

	// Called before the first frame
	 bool Start();

	bool Update(float dt);

	// Called before quitting
	//bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });
	void DestroyGuiControl(GuiControl* entity);

	void AddGuiControl(GuiControl* entity);

public:

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* texture;

	int font;
	int font2;
	int font3;
	int font4;
	int font11;
	int font21;
	int font31;
	int font41;

};

#endif // __GUIMANAGER_H__
