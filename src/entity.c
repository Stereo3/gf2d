#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include "entity.h"

typedef struct
{
    Entity *entity_list;
    Uint32  entity_count;
}EntityManager;

static EntityManager entity_manager = {0};

void entity_system_close()
{
    entity_clear_all(NULL);
    if(entity_manager.entity_list) free(entity_manager.entity_list);
    memset(&entity_manager,0,sizeof(EntityManager));
    slog("entity_system closed");
}

void entity_clear_all(Entity *ignore)
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if(&entity_manager.entity_list[i] == ignore)continue;
        if(!entity_manager.entity_list[i]._inuse)continue;
        entity_free(&entity_manager.entity_list[i]);        
    }
    //free(entity_manager.entity_list);
}

void entity_hide_all(Entity *ignore)
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if(&entity_manager.entity_list[i] == ignore)continue;
        if(!entity_manager.entity_list[i]._inuse)continue;
        if(entity_manager.entity_list[i].isPlayer)continue;
        entity_manager.entity_list[i].hidden = 1;     
    }
}

void entity_reset_all(Entity *ignore)
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if(&entity_manager.entity_list[i] == ignore)continue;
        if(!entity_manager.entity_list[i]._inuse)continue;
        if(entity_manager.entity_list[i].isPlayer)continue;
        entity_manager.entity_list[i].position = entity_manager.entity_list[i].spawnPoint;     
    }
}

void entity_unhide_all(Entity *ignore)
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if(&entity_manager.entity_list[i] == ignore)continue;
        if(!entity_manager.entity_list[i]._inuse)continue;
        entity_manager.entity_list[i].hidden = 0;     
    }
}

void entity_system_init(Uint32 maxEntities)
{
    if(entity_manager.entity_list)
    {
        slog("An active entity manager already exsists");
        return;
    }
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity),maxEntities);
    if (entity_manager.entity_list == NULL)
    {
        slog("failed to allocate entity list, cannot allocate ZERO entities");
        return;
    }
    entity_manager.entity_count = maxEntities;
    atexit(entity_system_close);
    slog("entity_system initialized");
}


Entity *entity_new()
{
    
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)// not used yet, so we can!
        {
            entity_manager.entity_list[i]._inuse = 1;
            entity_manager.entity_list[i].health = 100;
            entity_manager.entity_list[i].isPlayer = 0;
            entity_manager.entity_list[i].isEnemy = 0;
            entity_manager.entity_list[i].isTown = 0;
            entity_manager.entity_list[i].isNpc = 0;
            entity_manager.entity_list[i].enemyType = 0;


            //slog("made new entity");
            return &entity_manager.entity_list[i];
        }
    }
    slog("entity_new: no free space in the entity list");
    return NULL;
}

void entity_free(Entity *self)
{
    if (!self)return;
    //MUST DESTROY
    gf2d_sprite_free(self->sprite);
    if(self->free)self->free(self->customData);
    memset(self,0,sizeof(Entity));
}

void entity_think(Entity *self)
{
    if(!self)return;
    if(self->health <= 0)entity_free(self);
    if(self->think)self->think(self);
}


void entity_think_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_think(&entity_manager.entity_list[i]);
    }    
}

void entity_update(Entity *self)
{
    if(!self)return;
    if(self->update)self->update(self);
}


void entity_update_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_update(&entity_manager.entity_list[i]);
    }    
}


void entity_draw(Entity *self)
{
    Vector2D drawPosition;

    vector2d_add(drawPosition, self->position,camera_get_offset());

    if(!self)return;
    if(self->hidden)return;
    if(self->sprite)
    {
        gf2d_sprite_render(
        self->sprite,
        drawPosition,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (Uint32) self->frame);
    }
}

void entity_draw_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        
        if (!entity_manager.entity_list[i]._inuse)continue;
        //slog("Entity to draw: %s", entity_manager.entity_list[i].entityName);
        //if (entity_manager.entity_list[i].PLACEHOLDER)continue;
        entity_draw(&entity_manager.entity_list[i]);
    }    
}

/* void entity_draw_to_surface_test(Entity *self, World *worldToDrawTo)
{
    if(!self)return;
    if(self->hidden)return;
    if(self->sprite)
    {
    gf2d_sprite_draw_to_surface(
    self->sprite,
    self->position,
    NULL,
    NULL,
    self->frame,
    worldToDrawTo->tileLayer->surface
    );
    }
} */

Entity *entity_get_player(void)
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        if(entity_manager.entity_list[i].isPlayer == 1)
        {
            return &entity_manager.entity_list[i];
        }
    }
    return NULL;

}

Entity *entity_get_npc(void)
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        if(entity_manager.entity_list[i].isNpc == 1)
        {
            return &entity_manager.entity_list[i];
        }
    }
    return NULL;
}

Entity *entity_get_enemy(void)
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        if(entity_manager.entity_list[i].isEnemy == 1)
        {
            return &entity_manager.entity_list[i];
        }
    }
    return NULL;
}

Uint8 entity_collide_check(Entity *self, Entity *other){

    if((!self) || (!other)||(self == other))return 0;

    Circle A;
    Circle B;

   

    vector2d_copy(A, self->position);
    //vector2d_add(A, A, self->bounds);
    A.r = self->bounds.r;

    vector2d_copy(B, other->position);
    //vector2d_add(B, B, other->bounds);
    B.r = other->bounds.r;

    //slog("Circle A Values: %f, %f, %f", A.x, A.y, A.r);
    //slog("Circle B Values: %f, %f, %f", B.x, B.y, B.r);
    return gfc_circle_overlap(A,B);
}


Entity *entity_get_collision_partner(Entity *self){
    int i;
    if(!self)return NULL;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        if(self == &entity_manager.entity_list[i])continue;

        if(entity_collide_check(self, &entity_manager.entity_list[i]) == 1)
        {
            //slog("Collided");
            return &entity_manager.entity_list[i];
            
        }

    }
    return NULL;
}

int rng_machine(int lower, int upper, int count)
{
    int i;
    srand(time(0)); 
    for (i = 0; i < count; i++) { 
        int num = (rand() % 
        (upper - lower + 1)) + lower; 
        //slog("RNG: %i", num);
        return num; 
    } 
}

/* Vector2D calculate_new_camera_position()
{
    Entity *player = entity_get_player();
    Vector2D player_position = player->position;
    
    Camera *the_camera = get_camera();
    Vector2D target_position;

    target_position.x = player_position.x - (the_camera->size.x * 0.5);
    target_position.y = player_position.y - (the_camera->size.y * 0.5);
    
    return target_position;
} */


/* void update_enemy_positions()
{
    int i, e, enemy_count;

    Vector2D old_camera_position = camera_get_position();

    Vector2D new_camera_position = calculate_new_camera_position();

    Vector2D position_difference;
    vector2d_sub(position_difference,new_camera_position,old_camera_position);

    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        if(entity_manager.entity_list[i].isEnemy == 1)
        {
            enemy_count++;
        }
    }

    for (e = 0; e < enemy_count; e++)
    {
        Vector2D enemy_position = get_enemy_position(i);
        enemy_position = vector2d_add(enemy_position, position_difference);
        set_enemy_position(i, enemy_position);
    }
    
}
 */


/* Uint8 bad_collision_check(Entity *self, Entity *other)
{
    int i;
    if(!self)return NULL;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        if(self == &entity_manager.entity_list[i])continue;

        if(&self->position == &other->position)
        {
            return 1;
            slog("Collided");
        }

    }
} */


//entity draw modifications from: https://github.com/engineerOfLies/gameframework2d/blob/master/src/gf2d_entity.c

