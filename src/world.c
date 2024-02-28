#include "simple_json.h"
#include "simple_logger.h"
#include "world.h"

World *world_test_new()
{
    int i;
    int width = 75, height = 45;
    World *world;

    world = world_new(width, height);

    if(!world)return NULL;

    world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    world->tileSet = gf2d_sprite_load_all(
        "images/backgrounds/tileset.png",
        16,
        16,
        1,
        1);
    for(i = 0; i < width; i++)
    {
        world->tileMap[i] = 1;
        world->tileMap[i + ((height - 1) * width)] = 1;
    }    
    for(i = 0; i < height; i++)
    {
        world->tileMap[i * width] = 1;
        world->tileMap[i * width + (width - 1)] = 1;
    }
    return world;
}

World *world_new(Uint32 width, Uint32 height)
{
    World *world;

    if((!width)||(!height))
    {
        slog("Cannot make a world with zero width and height");
        return NULL;
    }

    world = gfc_allocate_array(sizeof(World),1);
    if(!world)
    {
        slog("failed to allocate a new world");
        return NULL;
    }

    world->tileMap = gfc_allocate_array(sizeof(Uint8), height*width);
    world->tileHeight = height;
    world->tileWidth = width;
    return world;
}

void world_free(World *world)
{
    if(!world)return;

    gf2d_sprite_free(world->background);
    gf2d_sprite_free(world->tileSet);
    free(world->tileMap);
    free(world);
}

void world_draw(World *world)
{
    int i,j;
    int index;
    int frame;
    Vector2D position;
    if(!world)return;
    gf2d_sprite_draw_image(world->background,vector2d(0,0));
    if(!world->tileSet)return;
    for(j=0;j < world->tileHeight;j++)
    {
        for(i=0;i < world->tileWidth;i++)
        {
            index = i + (j*world->tileWidth);
            if(world->tileMap[index] == 0)continue;
            position.x = i*world->tileSet->frame_w;
            position.y = i*world->tileSet->frame_h;
            frame = world->tileMap[index] - 1;
            gf2d_sprite_draw(
                world->tileSet,
                position,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                frame);
        }
    }
}
