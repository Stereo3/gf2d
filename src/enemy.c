#include "enemy.h"

Entity *enemy_new(Vector2D enemyPosition)
{
    Entity *enemy;

    enemy = entity_new();

    if(!enemy)
    {
        slog("Failed to create new enemy");
        return NULL;
    }

    enemy->sprite = gf2d_sprite_load_all("images/pirateship.png",178,214,4,0);
    enemy->isEnemy = 1;
    enemy->frame = 5;
    enemy->position = enemyPosition;


    return enemy;
}

void enemy_think(Entity *self)
{
    if(!self)return;

    Vector2D up, down, left, right;
    up = vector2d(0,-enemySpeed);
    down = vector2d(0,enemySpeed);
    left = vector2d(-enemySpeed,0);
    right = vector2d(enemySpeed,0);

    if(movementBudget_x > 0 || movementBudget_y > 0)
    {

    }
}

void enemy_update(Entity *self)
{
    if(!self)return;

    self->frame += 0.1;
    if(self->frame >= 8)self->frame = 5;

}

void enemy_free(Entity *self)
{
    if(!self)return;
    entity_free(self);
}