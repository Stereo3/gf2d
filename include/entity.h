#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gf2d_sprite.h"
#include "gfc_types.h"
#include "gfc_primitives.h"
#include "simple_logger.h"
#include "gfc_shape.h"
#include "camera.h"
#include "world.h"


typedef struct Entity_S
{
    Uint8 _inuse;
    Sprite *sprite;
    Uint8 hidden;
    Uint8 selected;
    Uint8 isPlayer, isEnemy, isTown, isNpc, isItem;
    Uint8 hasAttacked;
    Uint8 beingTalkedTo;
    float frame;
    Circle bounds;
    Uint32 enemyType;
    Uint8 lastDialougeOption;

    Vector2D position, lastPosition, spawnPoint;
    
    const char *entityName;
    const char *sayTheLine;
    
    void       (*think)(struct Entity_S *self);
    void       (*update)(struct Entity_S *self);
    void       (*free)(struct Entity_S *self);

    //struct Entity_S *

    void *customData;

    int health, healthPool;
    Uint8 isAlive;

}Entity;


/**
 * @brief initializes the entity subsystem
 * @param maxEntities the limit on number of entities that can exist at the same time
 */
void entity_system_init(Uint32 maxEntities);

/**
 * @brief provide a pointer to a new empty entity
 * @return NULL on error or a valid entity pointer otherwise
 */
Entity *entity_new();

/**
 * @brief free a previously created entity from memory
 * @param self the entity in question
 */
void entity_free(Entity *self);


/**
 * @brief Draw an entity in the current frame
 * @param self the entity in question
 */
void entity_draw(Entity *self);

/**
 * @brief draw ALL active entities
 */
void entity_draw_all();

/**
 * @brief Call an entity's think function if it exists
 * @param self the entity in question
 */
void entity_think(Entity *self);

/**
 * @brief run the think functions for ALL active entities
 */
void entity_think_all();

/**
 * @brief run the update functions for ALL active entities
 */
void entity_update_all();

/**
 * @brief clean up all active entities
 * @param ignore do not clean up this entity
*/
void entity_clear_all(Entity *ignore);

Entity *entity_get_player(void);

Entity *entity_get_npc(void);

Entity *entity_get_enemy(void);

Uint8 entity_collide_check(Entity *self, Entity *other);
Entity *entity_get_collision_partner(Entity *self);

Uint8 bad_collision_check(Entity *self, Entity *other);

int rng_machine(int lower, int upper, int count); 

void entity_hide_all(Entity *ignore);

void entity_unhide_all(Entity *ignore);

void entity_reset_all(Entity *ignore);

//void update_enemy_positions();

//void entity_draw_to_surface_test(Entity *self, World *worldToDrawTo);




//Entity Class derived from my 3D game code, originally found here: https://github.com/engineerOfLies/gf3d/blob/main/include/entity.h
//rng_machine modified from: https://www.geeksforgeeks.org/generating-random-number-range-c/ 
//My (arguably worse) version found here: https://github.com/Stereo3/IT485-DayTar/blob/main/include/entity.h     
#endif