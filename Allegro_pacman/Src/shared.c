// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* titleFont = NULL;
int character_type = 1;
int game_type = 1;
int p1_key_left = ALLEGRO_KEY_A;
int p1_key_right = ALLEGRO_KEY_D;
int p1_key_up = ALLEGRO_KEY_W;
int p1_key_down = ALLEGRO_KEY_S;
int p2_key_left = ALLEGRO_KEY_LEFT;
int p2_key_right = ALLEGRO_KEY_RIGHT;
int p2_key_up = ALLEGRO_KEY_UP;
int p2_key_down = ALLEGRO_KEY_DOWN;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	titleFont = load_font("Assets/Minecraft.ttf", 48);
	switch (character_type) {
		case 1:
			themeMusic = load_audio("Assets/Music/original_theme.ogg");
			break;
		case 2:
			themeMusic = load_audio("Assets/Music/Erika.ogg");
			break;
		default:
			themeMusic = load_audio("Assets/Music/original_theme.ogg");
			break;
	}
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_font(titleFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
}