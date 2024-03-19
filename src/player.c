#include "simple_logger.h"
#include "player.h"
#include "camera.h"

Entity *collisionPartner;

static Player thePlayer = {0};

SDL_Event event;

Player *player_new(const char *thePlayerName)
{
    //slog("exsists?: %i", thePlayer.exsits);
    if(thePlayer.exsits)
    {
        slog("Player already exsists");
        return NULL;
    }
    

    thePlayer.exsits = 1;
    //slog("exsists?: %i", thePlayer.exsits);

    Entity *player;
    player = entity_new();
    //slog("player.c line 18");

    if(!player)
    {
        slog("Failed to create the player");
        return NULL;
    }



    player->sprite = gf2d_sprite_load_all("images/skelebones.png",64,64,9,0);
    player->frame = 19;
    player->position = vector2d(64,64);
    player->isPlayer = 1;
    player->entityName = thePlayerName;

    slog("The Players Chosen Name: %s \n", thePlayerName);

    thePlayer.player = player;
    thePlayer.playerName = thePlayerName;
    thePlayer.playerSpeed = 2;
    thePlayer.movementBudget_x = 128;
    thePlayer.movementBudget_y = 128;
    thePlayer.inCombat = 0;
    thePlayer.movementEnabled = 1;

    thePlayer.player->bounds.x = thePlayer.player->position.x;
    thePlayer.player->bounds.y = thePlayer.player->position.y;
    thePlayer.player->bounds.r = 64;
    thePlayer.player->health = 100;
    thePlayer.player->healthPool = thePlayer.player->health;
    thePlayer.player->hasAttacked = 0;

    //slog("Player Bounds Values: X:%f Y:%f R:%f", thePlayer.player->bounds.x,thePlayer.player->bounds.y,thePlayer.player->bounds.r);

    return &thePlayer;
}

void player_attack(Entity *target)
{
    //slog("Smack");
    target->health -= rng_machine(1,15,1);
    thePlayer.player->hasAttacked = 1;
    //slog("Enemy health: %i", target->health);
}

void player_think(Player *self)
{
    if(!self)return;
    
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL);
    Vector2D up, down, left, right;

    up = vector2d(0,-self->playerSpeed);
    down = vector2d(0,self->playerSpeed);
    left = vector2d(-self->playerSpeed,0);
    right = vector2d(self->playerSpeed,0);

    SDL_PollEvent(&event);
/* 
    switch(event.type)
    {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_w:
                    //slog("i pressed w");
                    self->player->position.y -= 64;
                    break;
                case SDLK_a:
                    self->player->position.x -= 64;
                    break;
                case SDLK_s:
                    self->player->position.y += 64;
                    break;
                case SDLK_d:
                    self->player->position.x += 64;
                    break;

            }
    }
 */
    switch(event.type)
    {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_k:
                    self->movementBudget_x = 128;
                    self->movementBudget_y = 128;
                    break;
            }
            break;
    }

    if(self->movementEnabled == 1)
    {
        if(self->movementBudget_x > 0 || self->movementBudget_y > 0)
        {
            if (keys[SDL_SCANCODE_W] && self->movementBudget_x > 0)
            {
                //vector3d_add(self->position,self->position,forward);
                vector2d_add(self->player->position,self->player->position,up);
                self->movementBudget_x -= 1;
            }
            if (keys[SDL_SCANCODE_S] && self->movementBudget_x > 0)
            {
                //vector3d_add(self->position,self->position,-forward);
                vector2d_add(self->player->position,self->player->position,down);
                self->movementBudget_x -= 1;
            }
            if (keys[SDL_SCANCODE_D] && self->movementBudget_y > 0)
            {
                //vector3d_add(self->position,self->position,right);
                vector2d_add(self->player->position,self->player->position,right);
                self->movementBudget_y -= 1;
            }
            if (keys[SDL_SCANCODE_A] && self->movementBudget_y > 0)
            {
                //vector3d_add(self->position,self->position,-right);
                vector2d_add(self->player->position,self->player->position,left);
                self->movementBudget_y -= 1;
            }

            if(keys[SDL_SCANCODE_N])
            {
                Entity *temp_ent;
                temp_ent = entity_new();

                temp_ent->sprite = gf2d_sprite_load_all("images/ed210.png",128,128,16,0);
                temp_ent->frame = 0;
                temp_ent->position = self->player->position;
                temp_ent->isEnemy = 1;
            }
            if(keys[SDL_SCANCODE_C])
            {
                entity_clear_all(self->player);
            }
        }
    }
    
    collisionPartner = entity_get_collision_partner(self->player);

    if(collisionPartner != NULL)
    {
        if(collisionPartner->isEnemy == 1)
        {
            self->movementEnabled = 0;
            vector2d_copy(self->lastLocation, self->player->position);
            //slog("1 Enemey Position: X:%f | Y:%f", collisionPartner->position.x, collisionPartner->position.y);
            self->player->position = vector2d(256,300);
            collisionPartner->position = vector2d(600,150);
            self->enemyInCombatWith = collisionPartner;
            self->inCombat = 1;
        }
    }
    

    if(self->enemyInCombatWith != NULL)
    { 
        if(self->inCombat == 1)
        {
            //slog("2 Enemey Position: X:%f | Y:%f", enemyInCombatWith->position.x, enemyInCombatWith->position.y);
            //slog("2 Enemey Position: X:%f | Y:%f", collisionPartner->position.x, collisionPartner->position.y);
            //slog(enemyInCombatWith->entityName);
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_o:
                            player_attack(self->enemyInCombatWith);
                            break;
                    }
                    break;
            }
        }
        else
        {
            vector2d_copy(self->player->position, self->lastLocation);
            self->movementEnabled = 1;
        }
    }
    


}
void player_update(Player *self)
{
    if(!self)return;
    self->player->frame += 0.1;
    //slog("Frame: %f", self->player->frame);
    if(self->player->frame >= 27)self->player->frame = 19;
    if(self->player->position.x <= 48)self->player->position.x = 48;
    if(self->player->position.x >= 1090)self->player->position.x = 1090;
    if(self->player->position.y <= 48)self->player->position.y = 48;
    if(self->player->position.y >= 592)self->player->position.y = 592;

    self->player->bounds.x = self->player->position.x;
    self->player->bounds.y = self->player->position.y;
    

    camera_center_on(self->player->position);
}
void player_free(Player *self)
{
    if(!self)return;
    entity_free(self->player);
    
    self->playerName = NULL;
    thePlayer.exsits = 0;
    slog("exsists?: %i | Player Freed", thePlayer.exsits);
}

Player *player_get_player()
{
    return &thePlayer;
}





/*eol@eof*/