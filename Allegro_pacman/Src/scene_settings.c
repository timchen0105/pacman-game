// TODO-HACKATHON 3-9: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "scene_menu_object.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO-IF: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static ALLEGRO_BITMAP* pacman_pic = NULL;
static ALLEGRO_BITMAP* jew_pic = NULL;

static Checkbox pacman_checkbox;
static Checkbox jew_checkbox;

static Checkbox normal_checkbox;
static Checkbox competition_checkbox;

static Slider music_vol_slider;

static Button p1_left_button;
static Button p1_right_button;
static Button p1_up_button;
static Button p1_down_button;

static void init(void) {
	pacman_checkbox = Checkbox_create(100, 130, 50, 50, character_type == 1 ? true : false, "Assets/checkbox.png",
		"Assets/checkbox2.png");
	jew_checkbox = Checkbox_create(100, 210, 50, 50, character_type == 2 ? true : false, "Assets/checkbox.png",
		"Assets/checkbox2.png");
	pacman_pic = load_bitmap("Assets/pacman_move.png");
	jew_pic = load_bitmap("Assets/Star_of_David_move.png");

	normal_checkbox = Checkbox_create(100, 330, 50, 50, game_type == 1 ? true : false, "Assets/checkbox.png",
		"Assets/checkbox2.png");
	competition_checkbox = Checkbox_create(400, 330, 50, 50, game_type == 2 ? true : false, "Assets/checkbox.png",
		"Assets/checkbox2.png");

	music_vol_slider = Slider_create(100, 470, 600, 20, music_volume*100, "Assets/slider.png", "Assets/slider2.png");

	p1_right_button = button_create(300, 590, 50, 50, "Assets/little_button.png", "Assets/little_button2.png");
	p1_up_button = button_create(600, 590, 50, 50, "Assets/little_button.png", "Assets/little_button2.png");
	p1_left_button = button_create(300, 650, 50, 50, "Assets/little_button.png", "Assets/little_button2.png");
	p1_down_button = button_create(600, 650, 50, 50, "Assets/little_button.png", "Assets/little_button2.png");
}

static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H - 50, ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to menu"
	);

	al_draw_text(titleFont, al_map_rgb(255, 255, 0), SCREEN_W / 2, 50, ALLEGRO_ALIGN_CENTER,
		"CHOSE YOUR PACMAN!"
	);

	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 300, 140, ALLEGRO_ALIGN_LEFT,
		"Classic Pacman"
	);

	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 300, 220, ALLEGRO_ALIGN_LEFT,
		"WW2 Edition"
	);

	al_draw_scaled_bitmap(pacman_pic, 0, 0, 16, 16, 200, 130, 50, 50, 0);

	al_draw_scaled_bitmap(jew_pic, 0, 0, 16, 16, 200, 210, 50, 50, 0);

	al_draw_text(menuFont, al_map_rgb(255, 255, 0), SCREEN_W / 2, 280, ALLEGRO_ALIGN_CENTER,
		"Game Mode"
	);

	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 180, 340, ALLEGRO_ALIGN_LEFT,
		"Normal Mode"
	);

	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 480, 340, ALLEGRO_ALIGN_LEFT,
		"Competition Mode"
	);

	al_draw_text(menuFont, al_map_rgb(255, 255, 0), SCREEN_W / 2, 410, ALLEGRO_ALIGN_CENTER,
		"Music Vol."
	);

	char music_vol_string[5];
	sprintf_s(music_vol_string, 5, "%d", music_vol_slider.volume);
	al_draw_text(menuFont, al_map_rgb(255, 255, 0), 750, 470, ALLEGRO_ALIGN_CENTER,
		music_vol_string
	);

	drawCheckbox(pacman_checkbox);
	drawCheckbox(jew_checkbox);
	drawCheckbox(normal_checkbox);
	drawCheckbox(competition_checkbox);
	drawSlider(music_vol_slider);
	drawButton(p1_left_button);
	drawButton(p1_right_button);
	drawButton(p1_up_button);
	drawButton(p1_down_button);

	al_draw_text(menuFont, al_map_rgb(255, 255, 0), SCREEN_W/2, 520 + 12, ALLEGRO_ALIGN_CENTER,
		"Control Key"
	);

	char tmp_string[20];
	sprintf_s(tmp_string, 20, "RIGHT:       %c", p1_key_right+'A' - 1);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150-3, 590 + 12, ALLEGRO_ALIGN_LEFT,
		tmp_string
	);
	sprintf_s(tmp_string, 20, "UP:             %c", p1_key_up + 'A' - 1);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 450, 590 + 12, ALLEGRO_ALIGN_LEFT,
		tmp_string
	);
	sprintf_s(tmp_string, 20, "LEFT:         %c", p1_key_left + 'A' - 1);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150-3, 650 + 12, ALLEGRO_ALIGN_LEFT,
		tmp_string
	);
	sprintf_s(tmp_string, 20, "DOWN:        %c", p1_key_down + 'A' - 1);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 450, 650 + 12, ALLEGRO_ALIGN_LEFT,
		tmp_string
	);

}

static void destroy() {
	al_destroy_bitmap(pacman_checkbox.checked_img);
	al_destroy_bitmap(pacman_checkbox.default_img);
	al_destroy_bitmap(jew_checkbox.checked_img);
	al_destroy_bitmap(jew_checkbox.default_img);
	al_destroy_bitmap(normal_checkbox.checked_img);
	al_destroy_bitmap(normal_checkbox.default_img);
	al_destroy_bitmap(competition_checkbox.checked_img);
	al_destroy_bitmap(competition_checkbox.default_img);
	al_destroy_bitmap(music_vol_slider.hovered_img);
	al_destroy_bitmap(music_vol_slider.default_img);
	al_destroy_bitmap(p1_left_button.hovered_img);
	al_destroy_bitmap(p1_left_button.default_img);
	al_destroy_bitmap(p1_right_button.hovered_img);
	al_destroy_bitmap(p1_right_button.default_img);
	al_destroy_bitmap(p1_up_button.hovered_img);
	al_destroy_bitmap(p1_up_button.default_img);
	al_destroy_bitmap(p1_down_button.hovered_img);
	al_destroy_bitmap(p1_down_button.default_img);
}

static void on_key_down(int keycode) {
	if (keycode == ALLEGRO_KEY_ENTER) {
		game_change_scene(scene_menu_create());
	}
	else if (buttonHover(p1_left_button, mouse_x, mouse_y)) {
		p1_key_left = keycode;
	}
	else if (buttonHover(p1_right_button, mouse_x, mouse_y)) {
		p1_key_right = keycode;
	}
	else if (buttonHover(p1_up_button, mouse_x, mouse_y)) {
		p1_key_up = keycode;
	}
	else if (buttonHover(p1_down_button, mouse_x, mouse_y)) {
		p1_key_down = keycode;
	}
}

static void on_mouse_down(int btn, int x, int y, int dz) {
	if (pacman_checkbox.hovered) {
		pacman_checkbox.checked = true;
		jew_checkbox.checked = false;
		character_type = 1;
		themeMusic = load_audio("Assets/Music/original_theme.ogg");
		game_log("pacman_checkbox.checked");
	}
	else if (jew_checkbox.hovered) {
		pacman_checkbox.checked = false;
		jew_checkbox.checked = true;
		character_type = 2;
		themeMusic = load_audio("Assets/Music/Erika.ogg");
		game_log("jew_checkbox.checked");
	}
	else if (normal_checkbox.hovered) {
		normal_checkbox.checked = true;
		competition_checkbox.checked = false;
		game_type = 1;
	}
	else if (competition_checkbox.hovered) {
		normal_checkbox.checked = false;
		competition_checkbox.checked = true;
		game_type = 2;
	}
	else if (music_vol_slider.hovered) {
		music_vol_slider.clicked = true;
	}
}

static void on_mouse_up(int btn, int x, int y, int dz) {
	if (music_vol_slider.hovered) {
		music_vol_slider.clicked = false;
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	if (pnt_in_rect(mouse_x, mouse_y, pacman_checkbox.body) ){
		pacman_checkbox.hovered = true;
	}
	else if (pnt_in_rect(mouse_x, mouse_y, jew_checkbox.body)) {
		jew_checkbox.hovered = true;
	}
	else if (pnt_in_rect(mouse_x, mouse_y, normal_checkbox.body)) {
		normal_checkbox.hovered = true;
	}
	else if (pnt_in_rect(mouse_x, mouse_y, competition_checkbox.body)) {
		competition_checkbox.hovered = true;
	}
	else if (pnt_in_rect(mouse_x, mouse_y, music_vol_slider.body)) {
		music_vol_slider.hovered = true;
	}
	else if (buttonHover(p1_left_button, mouse_x, mouse_y)) {
		p1_left_button.hovered = true;
	}
	else if (buttonHover(p1_right_button, mouse_x, mouse_y)) {
		p1_right_button.hovered = true;
	}
	else if (buttonHover(p1_up_button, mouse_x, mouse_y)) {
		p1_up_button.hovered = true;
	}
	else if (buttonHover(p1_down_button, mouse_x, mouse_y)) {
		p1_down_button.hovered = true;
	}
	else {
		pacman_checkbox.hovered = false;
		jew_checkbox.hovered = false;
		normal_checkbox.hovered = false;
		competition_checkbox.hovered = false;
		music_vol_slider.hovered = false;
		p1_left_button.hovered = false;
		p1_right_button.hovered = false;
		p1_up_button.hovered = false;
		p1_down_button.hovered = false;
		music_vol_slider.clicked = false;
	}
	
	if (music_vol_slider.hovered && music_vol_slider.clicked) {
		int now_x = mouse_x;
		if (now_x < 100) now_x = 100;
		if (now_x > 700) now_x = 700;
		music_vol_slider.volume = (now_x - 100) / 6;
		music_volume = (float)music_vol_slider.volume * 0.01;
	}
}

/*
static void on_mouse_scroll(int a, int mouse_x, int mouse_y, int z) {
	if (music_vol_slider.hovered == true) {
		music_vol_slider.volume += z;
		if (music_vol_slider.volume > 100) {
			music_vol_slider.volume = 100;
		}
		else if (music_vol_slider.volume < 0) {
			music_vol_slider.volume = 0;
		}
		music_volume = (float)music_vol_slider.volume * 0.01;
	}
}
*/

// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	scene.on_mouse_up = &on_mouse_up;
	scene.on_mouse_move = &on_mouse_move;
	//scene.on_mouse_scroll = &on_mouse_scroll;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

// TODO-Graphical_Widget:
// Just suggestions, you can create your own usage.
	// Selecting BGM:
	// 1. Declare global variables for storing the BGM. (see `shared.h`, `shared.c`)
	// 2. Load the BGM in `shared.c`.
	// 3. Create dropdown menu for selecting BGM in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and play the BGM if the corresponding option is selected.

	// Adjusting Volume:
	// 1. Declare global variables for storing the volume. (see `shared.h`, `shared.c`)
	// 2. Create a slider for adjusting volume in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. You may use checkbox to switch between mute and unmute.
	// 3. Adjust the volume and play when the button is pressed.

	// Selecting Map:
	// 1. Preload the map to `shared.c`.
	// 2. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. For player experience, you may also create another scene between menu scene and game scene for selecting map.
	// 3. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and draw the map if the corresponding option is selected.
		// 4.1. Suggestions: You may use `al_draw_bitmap` to draw the map for previewing. 
							// But the map is too large to be drawn in original size. 
							// You might want to modify the function to allow scaling.