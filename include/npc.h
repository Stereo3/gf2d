#ifndef __NPC_H__
#define __NPC_H__

#include "simple_logger.h"
#include "world.h"
#include "entity.h"
#include "player.h"


void npc_think(Entity *self);
void npc_update(Entity *self);
void npc_free(Entity *self);

Entity *npc_new(Vector2D npcPosition, Uint8 hiddenWhenSpawned);

void npc_dialouge_tree(Entity *self);

#endif