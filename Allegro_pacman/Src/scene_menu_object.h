#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include "utility.h"
#include "shared.h"
#include "game.h"

typedef struct Button {
	RecArea body;
	ALLEGRO_BITMAP* default_img;
	ALLEGRO_BITMAP* hovered_img;
	bool hovered;
}Button;

typedef struct Checkbox {
	RecArea body;
	ALLEGRO_BITMAP* default_img;
	ALLEGRO_BITMAP* checked_img;
	bool checked;
	bool hovered;
}Checkbox;

typedef struct Slider {
	RecArea body;
	ALLEGRO_BITMAP* default_img;
	ALLEGRO_BITMAP* hovered_img;
	int volume;
	bool hovered;
	bool clicked;
}Slider;

Button button_create(float, float, float, float, const char*, const char*);
void drawButton(Button button);
bool buttonHover(Button, int, int);

Checkbox Checkbox_create(float, float, float, float, bool, const char*, const char*);
void drawCheckbox(Checkbox checkbox);

Slider Slider_create(float, float, float, float, int, const char*, const char*);
void drawSlider(Slider slider);


#endif
