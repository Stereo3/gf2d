#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef struct Player_S
{
    Uint8 exsits, inCombat, movementEnabled;
    Entity *player;
    Entity *enemyInCombatWith;
    const char *playerName;
    float playerSpeed, movementBudget_x, movementBudget_y;
    Vector2D lastLocation;
    int gold;
}Player;

void player_think(Player *self);
void player_update(Player *self);
void player_free(Player *self);

Player *player_new(const char *thePlayerName);

Player *player_get_player();

void player_attack(Entity *target);


#endif