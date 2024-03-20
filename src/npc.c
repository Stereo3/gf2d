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

    npc->sprite = gf2d_sprite_load_all("images/pirateship.png",178,214,4,0);
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
    if(self->beingTalkedTo == 1)npc_dialouge_tree();
}

const char * npc_dialouge_tree()
{
    Player *player;
    player = player_get_player();

    switch(player->chosenDialougeOption)
    {
        case 1:
            player->chosenDialougeOption = 0;
            return "Hey there sonny!";
            break;
        case 2:
            player->chosenDialougeOption = 0;
            return "What?";
            break;
    default:
        break;
    }
    return "How the hell did you get here?";
}

void npc_free(Entity *self)
{
    entity_free(self);
}