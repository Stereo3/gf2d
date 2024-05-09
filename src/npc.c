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
    npc->lastDialougeOption = 0;


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

 /*    switch(player->chosenDialougeOption)
    {
        case 1: 
            //player->chosenDialougeOption = 0;
            //slog("Chose 1");
            self->sayTheLine = "Hey there Beard! It's been a while!";
            slog("Chose: %i", player->chosenDialougeOption);
                switch (player->chosenDialougeOption)
                {
                case 3:
                    self->sayTheLine = "You know what? I Do!";
                    switch (player->chosenDialougeOption)
                    {
                    case 5:
                        self->sayTheLine = "Find and bring be back a golden idol for 5 gold dabloons!";
                        break;
                    }
                    break;
                case 4:
                    self->sayTheLine = "Stay safe out on them waters now!";
                    break;
                }
            break;
        case 2:
            player->chosenDialougeOption = 0;
            self->sayTheLine = "THE FUCK YOU SAY TO ME YOU LITTLE SHIT!?";
            //slog("Chose 2");
            break;
            
    } */
    if(player->chosenDialougeOption == 1 || self->lastDialougeOption == 1)
    {
        self->lastDialougeOption = 1;
        self->sayTheLine = "Hey there Beard! It's been a while!";
        if(player->chosenDialougeOption == 3 || self->lastDialougeOption == 3)
        {
            self->sayTheLine = "You know what? I Do!";
        }
        if(player->chosenDialougeOption == 4)
        {
            self->sayTheLine = "Find and bring be back a golden idol for 5 gold dabloons!";
        }
        if(player->chosenDialougeOption == 5)
        {
            self->sayTheLine = "Stay safe on them waters now!";
        }
        slog("Chosen: %i", player->chosenDialougeOption);
    }
    else if(player->chosenDialougeOption == 2 || self->lastDialougeOption == 2)
    {
        self->lastDialougeOption = 2;
        self->sayTheLine = "THE FUCK YOU SAY TO ME YOU LITTLE SHIT!?";
    }
}

void npc_free(Entity *self)
{
    entity_free(self);
}