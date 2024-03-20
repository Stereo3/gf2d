#include "town.h"

Entity *town_new(Vector2D townPosition, const char * townName)
{
    Entity *town;

    town = entity_new();
    
    if(!town)
    {
        slog("unable to create town");
        return NULL;
    }

    town->position = townPosition;
    town->lastPosition = townPosition;

    town->bounds.x = townPosition.x;
    town->bounds.y = townPosition.y;
    town->bounds.r = 1;
    town->isTown = 1;
    town->entityName = townName;

    return town;
}

void town_free(Entity *self)
{
    entity_free(self);
}