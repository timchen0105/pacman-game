
#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"
#include "shared.h"
/* Shared variables */
#define GO_OUT_TIME 256
extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;

/* Declare static function prototypes */
// static function reference: https://stackoverflow.com/questions/558122/what-is-a-static-function-in-c
static void ghost_move_script_FREEDOM_random(Ghost* ghost, Map* M);
static void ghost_move_script_FREEDOM_shortest_path(Ghost* ghost, Map* M, Pacman* pman);
static void ghost_move_script_FREEDOM_priority_path(Ghost* ghost, Map* M);
static void ghost_move_script_BLOCKED(Ghost* ghost, Map* M);
static void ghost_move_script_GO_IN(Ghost* ghost, Map* M);
static void ghost_move_script_GO_OUT(Ghost* ghost, Map* M);
static void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman* const pacman);

static void ghost_move_script_FREEDOM_random(Ghost* ghost, Map* M) {
	if (game_type == 2) {
		return;
	}
	
	Directions counter_one = RIGHT;
	switch(ghost->objData.preMove) {
		case RIGHT:
			counter_one = LEFT;
			break;
		case LEFT:
			counter_one = RIGHT;
			break;
		case UP:
			counter_one = DOWN;
			break;
		case DOWN:
			counter_one = UP;
			break;
		default:
			counter_one = RIGHT;
			break;
	}

	static Directions proba[4]; // possible movement
	int cnt = 0;
	for (Directions i = 1; i <= 4; i++)
		if (i != counter_one && ghost_movable(ghost, M, i, ghost->status)) 	proba[cnt++] = i;
	if (cnt >= 1) {
		ghost_NextMove(ghost, proba[generateRandomNumber(0, cnt)]);
	}
	else { // for the dead end case
		ghost_NextMove(ghost, counter_one);
	}
	
}

static void ghost_move_script_FREEDOM_shortest_path(Ghost* ghost, Map* M, Pacman* pman)
{	
	if (game_type == 2) {
		return;
	}
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pman->objData.Coord.x, pman->objData.Coord.y);
}

static void ghost_move_script_FREEDOM_priority_path(Ghost* ghost, Map* M) {
	if (game_type == 2) {
		return;
	}

	Directions counter_one = RIGHT;
	switch (ghost->objData.preMove) {
	case RIGHT:
		counter_one = LEFT;
		break;
	case LEFT:
		counter_one = RIGHT;
		break;
	case UP:
		counter_one = DOWN;
		break;
	case DOWN:
		counter_one = UP;
		break;
	default:
		counter_one = RIGHT;
		break;
	}

	static Directions proba[4]; // possible movement
	int cnt = 0;
	for (Directions i = 1; i <= 4; i++)
		if (i != counter_one && ghost_movable(ghost, M, i, ghost->status)) 	proba[cnt++] = i;
	if (cnt >= 1) {
		ghost_NextMove(ghost, proba[0]);
	}
	else { // for the dead end case
		ghost_NextMove(ghost, counter_one);
	}

}

static void ghost_move_script_BLOCKED(Ghost* ghost, Map* M) {

	switch (ghost->objData.preMove)
	{
	case UP:
		if (ghost->objData.Coord.y == 10)
			ghost_NextMove(ghost, DOWN);
		else
			ghost_NextMove(ghost, UP);
		break;
	case DOWN:
		if (ghost->objData.Coord.y == 12)
			ghost_NextMove(ghost, UP);
		else
			ghost_NextMove(ghost, DOWN);
		break;
	default:
		ghost_NextMove(ghost, UP);
		break;
	}
}

static void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}

static void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') 
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}

static void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman * const pacman) {
	if (game_type == 2) {
		return;
	}
	
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
	// TODO-PB: escape from pacman
	// Description:
	// The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
	// To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
	// Then we choose other available direction rather than direction K.
	// In this way, ghost will escape from pacman.
	Directions counter_one = RIGHT;
	switch (ghost->objData.preMove) {
	case RIGHT:
		counter_one = LEFT;
		break;
	case LEFT:
		counter_one = RIGHT;
		break;
	case UP:
		counter_one = DOWN;
		break;
	case DOWN:
		counter_one = UP;
		break;
	default:
		counter_one = RIGHT;
		break;
	}

	static Directions proba[4]; // possible movement
	int cnt = 0;
	for (Directions i = 1; i <= 4; i++)
		if (i != shortestDirection && i != counter_one && ghost_movable(ghost, M, i, ghost->status)) 	proba[cnt++] = i;
	if (cnt >= 1) {
		ghost_NextMove(ghost, proba[generateRandomNumber(0, cnt)]);
	}
	else { // for the dead end case
		ghost_NextMove(ghost, counter_one);
	}
}

void ghost_move_script_Blinky(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
		switch (ghost->status)
		{
		case BLOCKED:
			ghost_move_script_BLOCKED(ghost, M);
			if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
				ghost->status = GO_OUT;
			break;
		case FREEDOM:
			ghost_move_script_FREEDOM_random(ghost, M);
			break;
		case GO_OUT:
			ghost_move_script_GO_OUT(ghost, M);
			break;
		case GO_IN:
			ghost_move_script_GO_IN(ghost, M);
			if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
				ghost->status = GO_OUT;
				ghost->speed = 2; // reset the speed after back to the cage.
			}
			break;
		case FLEE:
			ghost_move_script_FLEE(ghost, M, pacman);
			break;
		default:
			break;
		}

		if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
			ghost->objData.preMove = ghost->objData.nextTryMove;
			ghost->objData.nextTryMove = NONE;
		}
		else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
			return;

		switch (ghost->objData.preMove) {
		case RIGHT:
			ghost->objData.Coord.x += 1;
			break;
		case LEFT:
			ghost->objData.Coord.x -= 1;
			break;
		case UP:
			ghost->objData.Coord.y -= 1;
			break;
		case DOWN:
			ghost->objData.Coord.y += 1;
			break;
		default:
			break;
		}
		ghost->objData.facing = ghost->objData.preMove;
		ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_move_script_Pinky(Ghost* ghost, Map* M, Pacman* pacman) {
	// TODO-GC-movement: do a little modification on shortest path move script
	// Since always shortest path strategy is too strong, player have no chance to win this.
	// hint: Do shortest path sometime and do random move sometime.
	if (!movetime(ghost->speed))
		return;
		
		int random_number = generateRandomNumber(0, 4);
		switch (ghost->status)
		{
		case BLOCKED:
			ghost_move_script_BLOCKED(ghost, M);
			if (al_get_timer_count(game_tick_timer) - ghost->go_in_time > GO_OUT_TIME)
				ghost->status = GO_OUT;
			break;
		case FREEDOM:
			if (random_number == 0) {
				ghost_move_script_FREEDOM_random(ghost, M);
			}
			else {
				ghost_move_script_FREEDOM_shortest_path(ghost, M, pacman);
			}
			break;
		case GO_OUT:
			ghost_move_script_GO_OUT(ghost, M);
			break;
		case GO_IN:
			ghost_move_script_GO_IN(ghost, M);
			if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
				ghost->status = BLOCKED;
				ghost->speed = 2; // reset the speed after back to the cage.
				ghost->go_in_time = al_get_timer_count(game_tick_timer); 
			}
			break;
		case FLEE:
			ghost_move_script_FLEE(ghost, M, pacman);
			break;
		default:
			break;
		}

		if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
			ghost->objData.preMove = ghost->objData.nextTryMove;
			ghost->objData.nextTryMove = NONE;
		}
		else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
			return;

		switch (ghost->objData.preMove) {
		case RIGHT:
			ghost->objData.Coord.x += 1;
			break;
		case LEFT:
			ghost->objData.Coord.x -= 1;
			break;
		case UP:
			ghost->objData.Coord.y -= 1;
			break;
		case DOWN:
			ghost->objData.Coord.y += 1;
			break;
		default:
			break;
		}
		ghost->objData.facing = ghost->objData.preMove;
		ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_move_script_Inky(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;

	switch (ghost->status)
	{
	case BLOCKED:
		ghost_move_script_BLOCKED(ghost, M);
		if (al_get_timer_count(game_tick_timer) - ghost->go_in_time > GO_OUT_TIME)
			ghost->status = GO_OUT;
		break;
	case FREEDOM:
		RecArea pacman_area = getDrawArea(&(pacman->objData), GAME_TICK_CD);
		RecArea ghost_area = getDrawArea(&(ghost->objData), GAME_TICK_CD);
		ghost_area.x -= 100;
		ghost_area.y -= 100;
		ghost_area.w += 200;
		ghost_area.h += 200;
		if (RecAreaOverlap(&pacman_area, &ghost_area)) {
			ghost_move_script_FREEDOM_random(ghost, M);
			//game_log("check");
		}
		else {
			ghost_move_script_FREEDOM_shortest_path(ghost, M, pacman);
		}
		break;
	case GO_OUT:
		ghost_move_script_GO_OUT(ghost, M);
		break;
	case GO_IN:
		ghost_move_script_GO_IN(ghost, M);
		if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
			ghost->status = BLOCKED;
			ghost->speed = 2; // reset the speed after back to the cage.
			ghost->go_in_time = al_get_timer_count(game_tick_timer);
		}
		break;
	case FLEE:
		ghost_move_script_FLEE(ghost, M, pacman);
		break;
	default:
		break;
	}

	if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
		ghost->objData.preMove = ghost->objData.nextTryMove;
		ghost->objData.nextTryMove = NONE;
	}
	else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
		return;

	switch (ghost->objData.preMove) {
	case RIGHT:
		ghost->objData.Coord.x += 1;
		break;
	case LEFT:
		ghost->objData.Coord.x -= 1;
		break;
	case UP:
		ghost->objData.Coord.y -= 1;
		break;
	case DOWN:
		ghost->objData.Coord.y += 1;
		break;
	default:
		break;
	}
	ghost->objData.facing = ghost->objData.preMove;
	ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_move_script_Clyde(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;

	switch (ghost->status)
	{
	case BLOCKED:
		ghost_move_script_BLOCKED(ghost, M);
		if (al_get_timer_count(game_tick_timer) - ghost->go_in_time > GO_OUT_TIME)
			ghost->status = GO_OUT;
		break;
	case FREEDOM:
		ghost_move_script_FREEDOM_priority_path(ghost, M);
		break;
	case GO_OUT:
		ghost_move_script_GO_OUT(ghost, M);
		break;
	case GO_IN:
		ghost_move_script_GO_IN(ghost, M);
		if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
			ghost->status = BLOCKED;
			ghost->speed = 2; // reset the speed after back to the cage.
			ghost->go_in_time = al_get_timer_count(game_tick_timer);
		}
		break;
	case FLEE:
		ghost_move_script_FLEE(ghost, M, pacman);
		break;
	default:
		break;
	}

	if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
		ghost->objData.preMove = ghost->objData.nextTryMove;
		ghost->objData.nextTryMove = NONE;
	}
	else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
		return;

	switch (ghost->objData.preMove) {
	case RIGHT:
		ghost->objData.Coord.x += 1;
		break;
	case LEFT:
		ghost->objData.Coord.x -= 1;
		break;
	case UP:
		ghost->objData.Coord.y -= 1;
		break;
	case DOWN:
		ghost->objData.Coord.y += 1;
		break;
	default:
		break;
	}
	ghost->objData.facing = ghost->objData.preMove;
	ghost->objData.moveCD = GAME_TICK_CD;
}