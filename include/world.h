#ifndef __WORLD_H__
#define __WORLD_H__

#include "gf2d_sprite.h"

typedef struct
{
    Sprite *background;
    Sprite *tileSet;
    Uint8  *tileMap;
    Uint32  tileHeight;
    Uint32  tileWidth;

}World;

World *world_test_new();

World *world_new(Uint32 width, Uint32 height);

void world_free(World *world);

void world_draw(World *world);







#endif