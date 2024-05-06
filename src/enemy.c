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
    enemy->healthPool = enemy->health;
    enemy->hasAttacked = 0;
    enemy->isAlive = 1;
    
    enemy->bounds.r = 16;
    enemy->bounds.x = enemyPosition.x;
    enemy->bounds.y = enemyPosition.y;
    //enemy->entityName = "Steven";

    //slog("Enemy Bounds Values: X:%f Y:%f R:%f", enemy->bounds.x,enemy->bounds.y,enemy->bounds.r);

    return enemy;
}

void enemy_attack(Entity *target)
{
    //slog("Smack");
    Player *player;
    player = player_get_player();
    if(!player)return;

    target->health -= rng_machine(1,10,1);
    player->player->hasAttacked = 0;
    //slog("Enemy health: %i", target->health);
}

void enemy_think(Entity *self)
{
    if(!self)return;

/*     Vector2D up, down, left, right;
    up = vector2d(0,-enemySpeed);
    down = vector2d(0,enemySpeed);
    left = vector2d(-enemySpeed,0);
    right = vector2d(enemySpeed,0); */

    self->bounds.x = self->position.x;
    self->bounds.y = self->position.y;

    /*     if(movementBudget_x > 0 || movementBudget_y > 0)
    {

    } */

}

void enemy_update(Entity *self)
{
    if(!self)return;
    
    Player *player;

    player = player_get_player();
    if(!player)return;

    self->frame += 0.01;
    if(self->frame >= 8)self->frame = 5;
    if(self->health <= 0 && self->isAlive == 1)
    {
        self->isAlive = 0;
        enemy_die(self);
    }
    if(player->player->hasAttacked)
    {
        enemy_attack(player->player);
        self->hasAttacked = 1;
    }
}

void enemy_die(Entity *self)
{
    if(!self)return;

    Player *player;

    player = player_get_player();

    if(!player)return;

    player->gold += 10;
    player->inCombat = 0;
    player->movementBudget_x += 512;
    player->movementBudget_y += 512;
    enemy_free(self);
}

void enemy_free(Entity *self)
{
    if(!self)return;
    entity_free(self);
}

Vector2D get_enemy_position(Entity *self)
{
    return self->position;
}