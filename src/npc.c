#include "npc.h"

Entity *npc_new(Vector2D npcPosition, Uint8 hiddenWhenSpawned)
{
    Entity *npc;

    npc = entity_new();
    
    if(!npc)
    {
        slog("unable to create npc");
        return NULL;
    }

    npc->sprite = gf2d_sprite_load_all("images/characters/Merchant.png",50,56,4,0);
    npc->position = npcPosition;
    npc->lastPosition = npcPosition;

    npc->bounds.x = npcPosition.x;
    npc->bounds.y = npcPosition.y;
    npc->bounds.r = 1;
    npc->isNpc = 1;
    npc->hidden = hiddenWhenSpawned;


    return npc;
}

void npc_think(Entity *self)
{
    if(!self)return;
    //slog("Is Npc Talking?: %i", self->beingTalkedTo);
}

const char * npc_dialouge_tree()
{
    Player *player;
    player = player_get_player();

    switch(player->chosenDialougeOption)
    {
        case 1:
            player->chosenDialougeOption = 0;
            slog("Chose 1");
            return "Hey there sonny!";
            break;
        case 2:
            player->chosenDialougeOption = 0;
            slog("Chose 2");
            return "What?";
            break;
    default:
        break;
    }
    return "";
}

void npc_free(Entity *self)
{
    entity_free(self);
}