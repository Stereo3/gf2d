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
    enemy->health = 100;
    
    enemy->bounds.r = 16;
    enemy->bounds.x = enemyPosition.x;
    enemy->bounds.y = enemyPosition.y;
    enemy->entityName = "Steven";

    //slog("Enemy Bounds Values: X:%f Y:%f R:%f", enemy->bounds.x,enemy->bounds.y,enemy->bounds.r);

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

    self->bounds.x = self->position.x;
    self->bounds.y = self->position.y;

    if(movementBudget_x > 0 || movementBudget_y > 0)
    {

    }

}

void enemy_update(Entity *self)
{
    if(!self)return;

    self->frame += 0.01;
    if(self->frame >= 8)self->frame = 5;
    if(self->health <= 0)enemy_die(self);
}

void enemy_die(Entity *self)
{
    Player *player;

    player = player_get_player();

    player->gold += 10;
    player->inCombat = 0;
    player->movementBudget_x += 512;
    player->movementBudget_x += 512;


    enemy_free(self);
}

void enemy_free(Entity *self)
{
    if(!self)return;
    entity_free(self);
}