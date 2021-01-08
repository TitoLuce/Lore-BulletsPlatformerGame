#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Module.h"

#include "Point.h"
#include "SString.h"
#include "App.h"
#include "Audio.h"

#include "SDL/include/SDL.h"

enum class GuiControlType
{
    BUTTON,
    TOGGLE,
    CHECKBOX,
    SLIDER,
    SLIDERBAR,
    COMBOBOX,
    DROPDOWNBOX,
    INPUTBOX,
    VALUEBOX,
    SPINNER
};

enum class GuiControlState
{
    DISABLED,
    NORMAL,
    FOCUSED,
    PRESSED,
    SELECTED
};

class GuiControl
{
public:

    GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

    GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
        type(type),
        state(GuiControlState::NORMAL),
        bounds(bounds),
        text(text) 
    {
        color.r = 255; color.g = 255; color.b = 255;
        texture = NULL;
    }

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool Draw() const
    {
        return true;
    }

    void SetTexture(SDL_Texture* tex)
    {
        texture = tex;
        section = { 0, 0, 0, 0 };
    }

    void SetObserver(Module* module)
    {
        observer = module;
    }

    void NotifyObserver()
    {
        observer->OnGuiMouseClickEvent(this);
    }

	void SetFont(int aFont, int bFont, int cFont, int dFont, int a1Font, int b1Font, int c1Font, int d1Font)
	{
		font = aFont;
		font2 = bFont;
		font3 = cFont;
		font4 = dFont;
		font11 = a1Font;
		font21 = b1Font;
		font31 = c1Font;
		font41 = d1Font;
	}

public:

    uint32 id;
    GuiControlType type;
    GuiControlState state;

    const char* text;           // Control text (if required)
    SDL_Rect bounds;        // Position and size
	SDL_Rect sliderBounds;
    SDL_Color color;        // Tint color

    SDL_Texture* texture;   // Texture atlas reference
    SDL_Rect section;       // Texture atlas base section

    //Font font;              // Text font
	int font;
	int font2;
	int font3;
	int font4;
	int font11;
	int font21;
	int font31;
	int font41;

	unsigned int click = app->audio->LoadFx("Assets/Audio/Fx/click.ogg");
	unsigned int hover = app->audio->LoadFx("Assets/Audio/Fx/hover.ogg");

    Module* observer;        // Observer module (it should probably be an array/list)
};

#endif // __GUICONTROL_H__