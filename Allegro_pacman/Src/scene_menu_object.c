#define _CRT_SECURE_NO_WARNINGS

#include "scene_menu_object.h"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Button button_create(float x, float y, float w, float h, const char* default_image_path, const char* hovered_image_path) {

	Button button;
	memset(&button, 0, sizeof(Button));

	button.default_img = load_bitmap(default_image_path);
	if (hovered_image_path) {
		button.hovered_img = load_bitmap(hovered_image_path);
	}

	if (!button.default_img) {
		game_log("failed loading button image!");
	}

	button.body.x = x;
	button.body.y = y;
	button.body.w = w;
	button.body.h = h;

	return button;
}

void drawButton(Button button) {
	ALLEGRO_BITMAP* _img = button.hovered_img ? 
												button.hovered ? 
													button.hovered_img : 
													button.default_img : 
												button.default_img;
	al_draw_scaled_bitmap(
		_img,
		0, 0,
		al_get_bitmap_width(_img), al_get_bitmap_height(_img),
		button.body.x, button.body.y,
		button.body.w, button.body.h, 0
	);
}

bool buttonHover(Button button, int mouse_x, int mouse_y) {
	// TODO-HACKATHON 3-6: Check if mouse is hovering on the button
	//	Uncomment and fill the code below
	
	return pnt_in_rect(mouse_x, mouse_y, button.body);
	
	return false;
}

Checkbox Checkbox_create(float x, float y, float w, float h, bool checked, const char* default_image_path, const char* checked_image_path) {
	Checkbox checkbox;
	memset(&checkbox, 0, sizeof(Checkbox));

	checkbox.default_img = load_bitmap(default_image_path);
	if (checked_image_path) {
		checkbox.checked_img = load_bitmap(checked_image_path);
	}

	if (!checkbox.default_img) {
		game_log("failed loading checkbox image!");
	}

	checkbox.body.x = x;
	checkbox.body.y = y;
	checkbox.body.w = w;
	checkbox.body.h = h;

	checkbox.checked = checked;

	return checkbox;
}

void drawCheckbox(Checkbox checkbox) {
	ALLEGRO_BITMAP* _img = checkbox.checked_img ?
													checkbox.checked ?
														checkbox.checked_img :
														checkbox.default_img :
													checkbox.default_img;
	al_draw_scaled_bitmap(
		_img,
		0, 0,
		162, 162,
		checkbox.body.x, checkbox.body.y,
		checkbox.body.w, checkbox.body.h, 0
	);
}

Slider Slider_create(float x, float y, float w, float h, int volume, const char* default_image_path, const char* hovered_image_path) {
	Slider slider;
	memset(&slider, 0, sizeof(Slider));

	slider.default_img = load_bitmap(default_image_path);
	if (hovered_image_path) {
		slider.hovered_img = load_bitmap(hovered_image_path);
	}

	if (!slider.default_img) {
		game_log("failed loading button image!");
	}

	slider.body.x = x;
	slider.body.y = y;
	slider.body.w = w;
	slider.body.h = h;

	slider.volume = volume;
	slider.clicked = false;

	return slider;
}

void drawSlider(Slider slider) {
	ALLEGRO_BITMAP* _img = slider.hovered_img ?
		slider.hovered ?
		slider.hovered_img :
		slider.default_img :
		slider.default_img;
	al_draw_scaled_bitmap(
		_img,
		0, 0,
		600, 20,
		slider.body.x, slider.body.y,
		6*slider.volume, slider.body.h, 0
	);
}