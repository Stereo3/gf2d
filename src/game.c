#include <SDL.h>
#include "simple_logger.h"
#include "simple_json.h"
#include <SDL_mixer.h>

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_config.h"
#include "gfc_audio.h"

#include "font.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "world.h"
#include "enemy.h"
#include "town.h"
#include "npc.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    World *world;
    World *combat, *combatPirate;
    World *town_w_1, *town_w_2;
    World *jungle_w_1, *ruins_w_1, *desert_w_1;
    Uint8 mainMenuBool = 1;
    Sprite *mainMenuImg;
    SJson *json, *saver;
    Sound *ambiance;

    Vector2D town1Spawnpoint, town2Spawnpoint, jungle1Spawnpoint,
    ruins1Spawnpoint, desert1Spawnpoint, ship1Spawnpoint, pirate1Spawnpoint, offscreen;    

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);

    Player *player;

    Entity *pirateShip1, *pirate1;

    Entity *town1, *town2, *jungle1, *ruins1, *desert1;

    Entity *npc1;

    Entity *goldenIdol;

    TextLine fps, player_pos, movementBudgets, combatStatus, 
    enemyHealth, playerHealth, npcDialouge, playerDialougeOption1, playerDialougeOption2,
    townStatus, questText1, questText2;

    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN %s ====---",argv[0]);
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    font_init();
    entity_system_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(vector2d(1200,720));         
    gfc_audio_init(128,32,0,32,1,0);

    json = sj_load("saves/save1.save");
    if(!json)
    {
        slog("failed to load save file");
        return NULL;
    }

    offscreen = vector2d(-1000,-1000);
    town1Spawnpoint = vector2d(832,288);
    town2Spawnpoint = vector2d(1408,688);
    jungle1Spawnpoint = vector2d(96,768);
    ruins1Spawnpoint = vector2d(1536,64);
    desert1Spawnpoint = vector2d(832,768);
    ship1Spawnpoint = vector2d(320,320);
    pirate1Spawnpoint = vector2d(640,640);

    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    player = player_new("greg");

    world = world_load("maps/testworld.map");
    combat = world_load("maps/combat.map");
    combatPirate = world_load("maps/combatPirate.map");
    town_w_1 = world_load("maps/town.map");
    town_w_2 = world_load("maps/town2.map");
    jungle_w_1 = world_load("maps/jungle1.map");
    ruins_w_1 = world_load("maps/ruins1.map");
    desert_w_1 = world_load("maps/desert1.map");

    pirateShip1 = enemy_new(ship1Spawnpoint, 0);
    pirate1 = enemy_new(offscreen, 1);
    //TEST = enemy_new(vector2d(320,520), 2);

    town1 = town_new(town1Spawnpoint, "firstville");
    town2 = town_new(town2Spawnpoint,"secondale");
    jungle1 = town_new(jungle1Spawnpoint, "jungle1");
    ruins1 = town_new(ruins1Spawnpoint,"ruins1");
    desert1 = town_new(desert1Spawnpoint,"desert1");

    npc1 = npc_new(offscreen, 0);

    goldenIdol = entity_new();
    //player->npcBeingTalkedTo = npc1;

    mainMenuImg = gf2d_sprite_load_all("images/mainmenu.png",125,300,1,0);
    world_setup_camera(world);
    ambiance = gfc_sound_load("music/bgm.mp3", 1,1);
    if(!ambiance)
    {
        //slog error why
        slog("AMBIANCE IS NULL RAHHHHHHHHH \n");
        return NULL;
    }
    gfc_sound_play(ambiance,5,.025,-1,-1);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        font_cleanup();
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        if(mainMenuBool == 1)
        {
            if(!mainMenuImg)
            {
                slog("No Main Menu for you!");
                return NULL;
            }
            gf2d_graphics_clear_screen();

            gf2d_sprite_draw_image(mainMenuImg,vector2d(500,100));

            gf2d_graphics_next_frame();
        
            if (keys[SDL_SCANCODE_3])
            {
                done = 1;
            }
            else if(keys[SDL_SCANCODE_1])
            {
                mainMenuBool = 0;
            } 
        }
        else
        {
            entity_think_all();
            entity_update_all();
            player_think(player);
            player_update(player);
            enemy_think(pirateShip1);
            enemy_update(pirateShip1);
            enemy_think(pirate1);
            enemy_update(pirate1);
            npc_think(npc1);
            
            gf2d_graphics_clear_screen();// clears drawing buffers
            // all drawing should happen betweem clear_screen and next_frame
                //backgrounds drawn first            
                if(player->inExploration == 1)
                {
                    if(player->resetPositions == 1)
                    {
                        entity_reset_all(NULL);
                        player->resetPositions = 0;
                    }
                    world_draw(world);
                    entity_unhide_all(npc1);
                }
                else if(player->inCombat == 1)
                {
                    switch (player->enemyInCombatWith->enemyType)
                    {
                    case 0:
                        world_draw(combat);
                        entity_hide_all(player->enemyInCombatWith);
                        gfc_line_sprintf(enemyHealth,"%i/%i",player->enemyInCombatWith->health,player->enemyInCombatWith->healthPool);
                        font_draw_text(enemyHealth,FS_large,GFC_COLOR_RED,vector2d(625,350));
                        gfc_line_sprintf(playerHealth,"%i/%i",player->player->health,player->player->healthPool);
                        font_draw_text(playerHealth,FS_large,GFC_COLOR_GREEN,vector2d(275,350));
                        break;
                    case 1:
                        world_draw(combatPirate);
                        entity_hide_all(player->enemyInCombatWith);
                        gfc_line_sprintf(enemyHealth,"%i/%i",player->enemyInCombatWith->health,player->enemyInCombatWith->healthPool);
                        font_draw_text(enemyHealth,FS_large,GFC_COLOR_RED,vector2d(675,375));
                        gfc_line_sprintf(playerHealth,"%i/%i",player->player->health,player->player->healthPool);
                        font_draw_text(playerHealth,FS_large,GFC_COLOR_GREEN,vector2d(375,375));
                        break;
                    default:
                        break;
                    }
                }
                else if(player->inTown == 1)
                {
                    if(gfc_stricmp(player->lastTownVisited->entityName, "firstville") == 0)
                    {
                    //slog("Town? : %i", player->inTown);
                        npc1->position = vector2d(512,350);
                        npc1->hidden = 0;
                        town2->position = offscreen;
                        jungle1->position = offscreen;
                        ruins1->position = offscreen;
                        desert1->position = offscreen;
                        
                        if(!pirateShip1)
                        {
                            slog("pirateShip1 is dead");
                        }
                        else
                        {
                            pirateShip1->position = offscreen;
                        }

                        if(!pirate1)
                        {
                            slog("pirate1 is dead");
                        }
                        else
                        {
                            pirate1->position = offscreen;
                        }
            
                        world_draw(town_w_1);
                        player->movementBudget_x = 100000;
                        player->movementBudget_y = 100000;
                        if(player->talkingToNpc == 1)
                        {
                            switch (player->chosenDialougeOption)
                            {
                            case 0:
                                gfc_line_sprintf(playerDialougeOption1, "1. Hey there... Steve?");
                                gfc_line_sprintf(playerDialougeOption2, "2. Skibidi rizz in Ohio, snarkle steez swag god the toilet.");
                                break;
                            case 1:
                                gfc_line_sprintf(playerDialougeOption1, "3. Got anything for me to do?");
                                gfc_line_sprintf(playerDialougeOption2, "");
                                break;
                            case 2:
                                gfc_line_sprintf(playerDialougeOption1, "");
                                gfc_line_sprintf(playerDialougeOption2, "");
                                break;
                            case 3:
                                gfc_line_sprintf(playerDialougeOption1, "4. ...Ok can you tell me?");
                                gfc_line_sprintf(playerDialougeOption2, "5. I'm leaving now.");
                                break;
                            case 4:
                                gfc_line_sprintf(player->quest3, "Find the golden Idol");
                                gfc_line_sprintf(playerDialougeOption1, "5. Bye now.");
                                gfc_line_sprintf(playerDialougeOption2, "");
                                break;
                            default:
                                gfc_line_sprintf(playerDialougeOption1, "1. Hey again Steve");
                                gfc_line_sprintf(playerDialougeOption2, "2. Skibidi rizz in Ohio, snarkle steez swag god the toilet.");
                                break;
                            }
                            
                            font_draw_text(playerDialougeOption1, FS_large,GFC_COLOR_GREEN,vector2d(0,35));
                            font_draw_text(playerDialougeOption2, FS_large,GFC_COLOR_RED,vector2d(0,80));

                            
                            if(npc1->beingTalkedTo == 1)
                            {
                                gfc_line_sprintf(npcDialouge, npc1->sayTheLine);
                                font_draw_text(npcDialouge,FS_large,GFC_COLOR_WHITE,vector2d(0,0));
                            }
                            
                        }
                    }
                    else if(gfc_stricmp(player->lastTownVisited->entityName, "secondale") == 0)
                    {
                        if(!pirateShip1)
                        {
                            slog("pirateShip1 is dead");
                        }
                        else
                        {
                            pirateShip1->position = offscreen;
                        }

                        if(!pirate1)
                        {
                            slog("pirate1 is dead");
                        }
                        else
                        {
                            pirate1->position = offscreen;
                        }
                        npc1->position = offscreen;
                        town1->position = offscreen;
                        jungle1->position = offscreen;
                        ruins1->position = offscreen;
                        desert1->position = offscreen;
                        world_draw(town_w_2);
                        player->movementBudget_x = 100000;
                        player->movementBudget_y = 100000;
                    }
                    else if(gfc_stricmp(player->lastTownVisited->entityName, "jungle1") == 0)
                    {
                        if(!pirateShip1)
                        {
                            slog("pirateShip1 is dead");
                        }
                        else
                        {
                            pirateShip1->position = offscreen;
                        }

                        if(!pirate1)
                        {
                            slog("pirate1 is dead");
                        }
                        else
                        {
                            pirate1->position = offscreen;
                        }
                        npc1->position = offscreen;
                        town1->position = offscreen;
                        town2->position = offscreen;
                        ruins1->position = offscreen;
                        desert1->position = offscreen;
                        world_draw(jungle_w_1);
                        player->movementBudget_x = 100000;
                        player->movementBudget_y = 100000;
                    }
                    else if(gfc_stricmp(player->lastTownVisited->entityName, "ruins1") == 0)
                    {
                        if(!pirateShip1)
                        {
                            slog("pirateShip1 is dead");
                        }
                        else
                        {
                            pirateShip1->position = offscreen;
                        }

                        if(!pirate1)
                        {
                            slog("pirate1 is dead");
                        }
                        else
                        {
                            pirate1->position = pirate1Spawnpoint;
                        }
                        npc1->position = offscreen;
                        town1->position = offscreen;
                        town2->position = offscreen;
                        jungle1->position = offscreen;
                        world_draw(ruins_w_1);
                        player->movementBudget_x = 100000;
                        player->movementBudget_y = 100000;
                    }
                    else if(gfc_stricmp(player->lastTownVisited->entityName, "desert1") == 0)
                    {
                        if(!pirateShip1)
                        {
                            slog("pirateShip1 is dead");
                        }
                        else
                        {
                            pirateShip1->position = offscreen;
                        }

                        if(!pirate1)
                        {
                            slog("pirate1 is dead");
                        }
                        else
                        {
                            pirate1->position = offscreen;
                        }
                        npc1->position = offscreen;
                        town1->position = offscreen;
                        town2->position = offscreen;
                        jungle1->position = offscreen;
                        ruins1->position = offscreen;
                        world_draw(desert_w_1);
                        player->movementBudget_x = 100000;
                        player->movementBudget_y = 100000;
                    }
                    else
                    {
                        slog("No town to visit");
                    }
                }
                else
                {
                    player->player->health = 0;
                    //world_draw(world);
                }
                //slog("Player in Combat: %i", player->inCombat);
            
                entity_draw_all();
                //entity_draw_to_surface_test(TEST, world);
                
                if(keys[SDL_SCANCODE_TAB])
                {
                    gfc_line_sprintf(player_pos,"Players Position| X: %f, Y: %f", player->player->position.x, player->player->position.y);
                    font_draw_text(player_pos,FS_small, GFC_COLOR_CYAN,vector2d(10,10));
                    gfc_line_sprintf(fps,"FPS: %f",gf2d_graphics_get_frames_per_second());
                    font_draw_text(fps,FS_small,GFC_COLOR_GREEN,vector2d(10,40));
                    gfc_line_sprintf(movementBudgets,"Movement Budgets | X: %f, Y: %f",player->movementBudget_x, player->movementBudget_y);
                    font_draw_text(movementBudgets,FS_small,GFC_COLOR_RED,vector2d(10,70));
                    gfc_line_sprintf(combatStatus,"Combat Status: %i",player->inCombat);
                    font_draw_text(combatStatus,FS_small,GFC_COLOR_ORANGE,vector2d(10,100));
                    gfc_line_sprintf(townStatus,"Town Status: %i",player->inTown);
                    font_draw_text(townStatus,FS_small,GFC_COLOR_ORANGE,vector2d(10,130));

                    gfc_line_sprintf(questText1,"Quests");
                    font_draw_text(questText1,FS_small, GFC_COLOR_YELLOW,vector2d(900,10));
                    gfc_line_sprintf(questText1,"------------------");
                    font_draw_text(questText1,FS_small, GFC_COLOR_YELLOW,vector2d(850,30));
                    font_draw_text(player->quest1,FS_small, GFC_COLOR_YELLOW,vector2d(700,60));
                    font_draw_text(player->quest2,FS_small, GFC_COLOR_YELLOW,vector2d(725,90));
                    font_draw_text(player->quest3,FS_small, GFC_COLOR_YELLOW,vector2d(825,120));


                }
                
                //UI elements last
                gf2d_sprite_draw(
                    mouse,
                    vector2d(mx,my),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &mouseColor,
                    (int)mf);

            gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        }
        
        if(player->player->health <= 0)
        {
            player->player->position = player->player->spawnPoint;
            player->resetPositions = 1;
            player->inExploration = 1;
            player->inCombat = 0;
            player->player->position = vector2d(64,64);
            player->player->health += 100;
            mainMenuBool = 1;
        }
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    saver = sj_object_get_value(json,"player_location");
    sj_object_delete_key(saver, "coordinates");
    sj_object_insert(saver,"coordinates",sj_vector2d_new(player->player->position));

    sj_save(json,"saves/save1.save");
    sj_free(saver);
    sj_free(json);
    player_free(player);
    enemy_free(pirateShip1);
    world_free(world);
    world_free(combat);
    mainMenuBool = 1;
    slog("---==== END %s ====---", argv[0]);
    return 0;
}
/*eol@eof*/