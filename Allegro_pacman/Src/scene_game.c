#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"


// TODO-HACKATHON 2-0: Create one ghost
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
// TODO-GC-ghost: create a least FOUR ghost!
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
bool game_over = false;

/* Internal variables*/
static ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static High_score_table* high_score_table;
static ALLEGRO_BITMAP* type_in_table;
static Ghost** ghosts;
static bool ctrl_pressing = false;
static bool high_score_table_mode = false;
static bool insert_name_mode = false;
static int control_ghost_num = 0;
static char name[20];
static char name_idx = -1;
bool debug_mode = false;
bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_key_up(int key_code);
static void on_mouse_down(int btn, int x, int y, int dz);
static void on_mouse_up(int btn, int x, int y, int dz);
static void render_init_screen(void);
static void draw_hitboxes(void);
static void draw_type_in_table(void);
static void update_score(void);

static void init(void) {
	game_over = false;
	game_main_Score = 0;
	for (int i = 0; i < 20; i++) {
		name[i] = '\0';
	}
	name_idx = -1;
	high_score_table_mode = false;
	insert_name_mode = false;
	ctrl_pressing = false;
	// create map
	basic_map = create_map(NULL);
	// TODO-GC-read_txt: Create map from .txt file so that you can design your own map!!
	basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	
	high_score_table = create_high_score_table("Assets/high_score_records.txt");
	if (!high_score_table) {
		game_abort("error on creating high score table");
	}

	type_in_table = load_bitmap("Assets/type_in_table.png");
	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// TODO-HACKATHON 2-1: Allocate dynamic memory for ghosts array.
	
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// TODO-HACKATHON 2-2: create a ghost.
		// Try to look the definition of ghost_create in ghost.h and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);  
			if (!ghosts[i])
				game_abort("error creating ghost\n");
			
		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick per second
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// TODO-HACKATHON 1-3: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, '.');
		game_main_Score += 1;
		basic_map->beansCount--;
		basic_map->map[Grid_y][Grid_x] = 0;
		break;
	case 'P':
		// TODO-GC-PB: ease power bean
		// pacman_eatItem(...);
		// stop and reset power_up_timer value
		// start the timer
		pacman_eatItem(pman, 'P');
		game_main_Score += 10;
		al_stop_timer(power_up_timer);
		al_set_timer_count(power_up_timer, 0);
		al_start_timer(power_up_timer);
		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], true);
		}
		basic_map->map[Grid_y][Grid_x] = 0;
		break;
	default:
		break;
	}
	
	// TODO-HACKATHON 1-4: erase the item you eat from map
	// Be careful, don't erase the wall block.
	
	//basic_map->map[Grid_y][Grid_x] = 0;
	
}
static void status_update(void) {
	// TODO-PB: check powerUp duration
	
	if (pman->powerUp)
	{
		// Check the value of power_up_timer
		// If runs out of time reset all relevant variables and ghost's status
		// hint: ghost_toggle_FLEE

		if (get_power_up_timer_tick() >= get_power_up_duration()) {
			al_stop_timer(power_up_timer);
			for (int i = 0; i < GHOST_NUM; i++) {
				ghost_toggle_FLEE(ghosts[i], false);
			}
			pman->powerUp = false;
		}
	}
	


	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN){
			continue;
		}
		else if (ghosts[i]->status == FREEDOM)
		{
			// TODO-GC-game_over: use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect if pacman and ghosts collide with each other.
			// And perform corresponding operations.
			// [NOTE] You should have some if-else branch here if you want to implement power bean mode.
			// Uncomment Following Code
			
			RecArea ghost_area = getDrawArea(&(ghosts[i]->objData), GAME_TICK_CD);
			RecArea pman_area = getDrawArea(&(pman->objData), GAME_TICK_CD);
			if(!cheat_mode && RecAreaOverlap(&ghost_area, &pman_area))
			{
					game_log("collide with ghost\n");
					al_rest(1.0);
					pacman_die();
					game_over = true;
					break; // animation shouldn't be trigger twice.
			}
			
		}
		else if (ghosts[i]->status == FLEE)
		{
			// TODO-GC-PB: if ghost is collided by pacman, it should go back to the cage immediately and come out after a period.
			
			RecArea ghost_area = getDrawArea(&(ghosts[i]->objData), GAME_TICK_CD);
			RecArea pman_area = getDrawArea(&(pman->objData), GAME_TICK_CD);
			if(!cheat_mode && RecAreaOverlap(&ghost_area, &pman_area))
			{
				ghost_collided(ghosts[i]);
			}
			
		}
	}
}

static void update(void) {

	if (game_over) {
		// TODO-GC-game_over: start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
		// hint: refer al_get_timer_started(...), al_get_timer_count(...), al_stop_timer(...), al_rest(...)
		
			// start the timer if it hasn't been started.
			// check timer counter.
			// stop the timer if counter reach desired time.
		if (insert_name_mode || high_score_table_mode) {
			return;
		}

		if (!al_get_timer_started(pman->death_anim_counter)) {
			game_log("start death anim timer");
			al_start_timer(pman->death_anim_counter);
		}
		else {
			if (al_get_timer_count(pman->death_anim_counter) > 16) {
				al_stop_timer(pman->death_anim_counter);
				insert_name_mode = true;
			}
		}
		return;
	}

	if (basic_map->beansCount <= 0) {
		if (insert_name_mode || high_score_table_mode) {
			return;
		}
		insert_name_mode = true;
		return;
	}

	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	
	// TODO-GC-scoring: Draw scoreboard, something your may need is sprinf();
	char* score_string = (char*)malloc(sizeof(char) * 15);
	sprintf_s(score_string, 15, "SCORE: %d", game_main_Score);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		10, 10,
		ALLEGRO_ALIGN_LEFT,
		score_string
	);
	free(score_string);

	draw_map(basic_map);

	pacman_draw(pman);
	
	if (insert_name_mode) {
		draw_type_in_table();
		return;
	}

	if (high_score_table_mode) {
		draw_high_score_table(high_score_table);
		return;
	}

	if (game_over) {
		return;
	}

		
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea((object *)pman, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	// TODO-GC-memory: free map array, Pacman and ghosts
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_destory(ghosts[i]);
	}
	pacman_destroy(pman);
	delete_map(basic_map);
	al_destroy_bitmap(high_score_table->picture);
	free(high_score_table);
	al_destroy_bitmap(type_in_table);
}

static void on_key_down(int key_code) {
	if (insert_name_mode && key_code != ALLEGRO_KEY_ENTER) {
		if (key_code == ALLEGRO_KEY_BACKSPACE) {
			if (name_idx >= 0) {
				name[name_idx] = '\0';
				name_idx--;
			}
		}
		else {
			name_idx++;
			name[name_idx] = key_code + 'A' - 1;
		}
		return;
	}

	if (key_code == ALLEGRO_KEY_LCTRL || key_code == ALLEGRO_KEY_RCTRL) {
		ctrl_pressing = true;
	}
	else if (key_code == p1_key_up) {
		pacman_NextMove(pman, UP);
	}
	else if (key_code == p1_key_left) {
		pacman_NextMove(pman, LEFT);
	}
	else if (key_code == p1_key_down) {
		if (ctrl_pressing) {
			for (int i = 0; i < GHOST_NUM; i++) {
				ghosts[i]->is_stop = !ghosts[i]->is_stop;
			}
		}
		else {
			pacman_NextMove(pman, DOWN);
		}
	}
	else if (key_code == p1_key_right) {
		pacman_NextMove(pman, RIGHT);
	}
	else if (game_type == 2 && key_code == p2_key_up) {
		ghost_NextMove(ghosts[control_ghost_num], UP);
	}
	else if (game_type == 2 && key_code == p2_key_left) {
		ghost_NextMove(ghosts[control_ghost_num], LEFT);
	}
	else if (game_type == 2 && key_code == p2_key_down) {
		ghost_NextMove(ghosts[control_ghost_num], DOWN);
	}
	else if (game_type == 2 && key_code == p2_key_right) {
		ghost_NextMove(ghosts[control_ghost_num], RIGHT);
	}
	else if (key_code == ALLEGRO_KEY_C) {
		if (game_type == 1) {
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else
				printf("cheat mode off\n");
		}
		else if (game_type == 2) {
			control_ghost_num++;
			if (control_ghost_num >= 4) control_ghost_num -= 4;
		}
	}
	else if (key_code == ALLEGRO_KEY_G) {
		debug_mode = !debug_mode;
	}
	else if (key_code == ALLEGRO_KEY_K) {
		for (int i = 0; i < GHOST_NUM; i++) {
			ghosts[i]->status = FLEE;
			ghost_collided(ghosts[i]);
		}
	}
	else if (key_code == ALLEGRO_KEY_L) {
		if (ctrl_pressing) {
			pman->can_cross_wall = !pman->can_cross_wall;
		}
	}
	else if (key_code == ALLEGRO_KEY_ENTER) {
		if (insert_name_mode) {
			insert_name_mode = false;
			update_score();
			high_score_table_mode = true;
		}
		else if (high_score_table_mode) {
			high_score_table_mode = false;
			store_score_records("Assets/high_score_records.txt", high_score_table);
			game_change_scene(scene_menu_create());
		}
	}

		// TODO-HACKATHON 1-1: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
}

static void on_key_up(int key_code) {
	if (key_code == ALLEGRO_KEY_LCTRL || key_code == ALLEGRO_KEY_RCTRL) {
		ctrl_pressing = false;
	}
}

static void on_mouse_down(int btn, int x, int y, int dz) {

	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_key_up = &on_key_up;
	scene.on_mouse_down = &on_mouse_down;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}

int64_t get_power_up_timer_tick() {
	return al_get_timer_count(power_up_timer);
}

int64_t get_power_up_duration() {
	return power_up_duration;
}

static void draw_type_in_table(void) {
	al_draw_bitmap(type_in_table, 150, 250, 0);
	al_draw_text(
		titleFont,
		al_map_rgb(0, 0, 0),
		SCREEN_W / 2, 300,
		ALLEGRO_ALIGN_CENTER,
		"INSERT YOUR NAME"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(0, 0, 0),
		SCREEN_W / 2, 400,
		ALLEGRO_ALIGN_CENTER,
		name
	);
	al_draw_text(
		menuFont,
		al_map_rgb(0, 0, 0),
		SCREEN_W / 2, 500,
		ALLEGRO_ALIGN_CENTER,
		"PRESS <ENTER> TO CONTINUE"
	);
}

static void update_score(void) {
	int rank = high_score_table->row_num;
	for (int i = 0; i < high_score_table->row_num; i++) {
		if (game_main_Score > high_score_table->records[i].score) {
			rank = i;
			break;
		}
	}
	if (rank >= 10) {
		return;
	}
	else {
		if (high_score_table->row_num + 1 <= 10) {
			high_score_table->row_num++;
		}
		for (int i = high_score_table->row_num - 1; i >= 0; i--) {
			if (i > rank) {
				high_score_table->records[i] = high_score_table->records[i-1];
			}
			else if (i == rank) {
				for (int j = 0; j < 20; j++) {
					high_score_table->records[i].name[j] = name[j];
				}
				high_score_table->records[i].score = game_main_Score;
			}
		}
	}
}