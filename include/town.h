#ifndef __TOWN_H__
#define __TOWN_H__

#include "simple_logger.h"
#include "world.h"
#include "entity.h"
#include "player.h"

void town_think(Entity *self);
void town_update(Entity *self);
void town_free(Entity *self);

 Entity *town_new(Vector2D townPosition);

#endif