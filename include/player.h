#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef struct Player_S
{
    Uint8 exsits, inCombat, movementEnabled, shipMode, inTown, talkingToNpc,
    inExploration, resetPositions;
    Uint8 chosenDialougeOption;
    Entity *player;
    Entity *enemyInCombatWith;
    Entity *lastTownVisited;
    Entity *npcBeingTalkedTo;
    const char *playerName;
    float playerSpeed, movementBudget_x, movementBudget_y;
    Vector2D lastLocation;
    TextLine quest1, quest2, quest3;
    int gold;
}Player;

void player_think(Player *self);
void player_update(Player *self);
void player_free(Player *self);

Player *player_new(const char *thePlayerName);

Player *player_get_player(void);

void player_attack(Entity *target);


/**
 * @brief a function to swap the players' sprite on the fly
 * @param self a pointer to the player
 * @param whatSprite an integer that correlates to a specififc sprite
 * 0 = Skelebones
 * 1 = Pirate ship
 * 3 = TBD
*/
void player_swap_sprite(Player *self, int whatSprite);

void player_talk_to_npc(Player *self, Entity *npcToTalkTo);

#endif