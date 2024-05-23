#ifndef EVENTS_H
#define EVENTS_H

#include "game.h"
#include "map.h"
#include "ARA_window.h"
#include <unistd.h>
#include <time.h>

void print_alphabet(Game *game, char car, int x, int y);
void print_bottom_window(Game *game);
void task_pop_up(Game *game, char * mission, char * emoji, int* x1, int* y1, int* x2, int* y2);
void task_recalibrate(Game *game);
void task_fill(Game *game);
void print_arena(Game *game, char car);
int QTE(Game *game);
void movementMonster(Game *game, int xmonster, int ymonster);
void monster(Game *game);

#endif