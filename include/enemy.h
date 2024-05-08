#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "simple_logger.h"
#include "world.h"
#include "entity.h"
#include "player.h"

//static float enemySpeed, movementBudget_x, movementBudget_y;

void enemy_think(Entity *self);
void enemy_update(Entity *self);
void enemy_free(Entity *self);
void enemy_die(Entity *self);
Vector2D get_enemy_position(Entity *self);

 Entity *enemy_new(Vector2D enemyPosition, Uint32 enemyType);

 void enemy_attack(Entity *target);

#endif