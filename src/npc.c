#include "npc.h"
#include "font.h"

Entity *npc_new(Vector2D npcPosition, Uint8 hiddenWhenSpawned)
{
    Entity *npc;

    npc = entity_new();
    
    if(!npc)
    {
        slog("unable to create npc");
        return NULL;
    }

    npc->sprite = gf2d_sprite_load_all("images/characters/Merchant.png",50,56,4,1);
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
    if(self->beingTalkedTo)npc_dialouge_tree(self);
}

void npc_dialouge_tree(Entity *self)
{
    Player *player;
    player = player_get_player();

    switch(player->chosenDialougeOption)
    {
        case 1:
            player->chosenDialougeOption = 0;
            slog("Chose 1");
            self->sayTheLine = "Hey there Beard! It's been a while!";
            break;
        case 2:
            player->chosenDialougeOption = 0;
            self->sayTheLine = "THE FUCK YOU SAY TO ME YOU LITTLE SHIT!?";
            slog("Chose 2");
            break;
    }
}

void npc_free(Entity *self)
{
    entity_free(self);
}