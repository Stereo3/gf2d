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
    World *combat;
    World *town_w_1, *town_w_2;
    Uint8 mainMenuBool = 1;
    Sprite *mainMenuImg;
    SJson *json, *saver;
    Sound *ambiance;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    Player *player;
    Entity *pirateShip1;
    Entity *town1, *town2;
    Entity *npc1;
    TextLine fps, player_pos, movementBudgets, combatStatus, 
    enemyHealth, playerHealth, npcDialouge, playerDialougeOption1, playerDialougeOption2;
    
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


    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    player = player_new("greg");
    world = world_load("maps/testworld.map");
    combat = world_load("maps/combat.map");
    town_w_1 = world_load("maps/town.map");
    //town_w_2 = world_load("maps/town2.map");
    pirateShip1 = enemy_new(vector2d(320,320));
    town1 = town_new(vector2d(710, 232), "firstville");
    //town2 = town_new(vector2d(972,522),"secondale");
    npc1 = npc_new(vector2d(512,350), 1);
    player->npcBeingTalkedTo = npc1;
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
            npc_think(npc1);
            
            gf2d_graphics_clear_screen();// clears drawing buffers
            // all drawing should happen betweem clear_screen and next_frame
                //backgrounds drawn first            
                if(player->inCombat == 1)
                {
                    world_draw(combat);
                    gfc_line_sprintf(enemyHealth,"%i/%i",player->enemyInCombatWith->health,player->enemyInCombatWith->healthPool);
                    font_draw_text(enemyHealth,FS_large,GFC_COLOR_RED,vector2d(625,350));
                    gfc_line_sprintf(playerHealth,"%i/%i",player->player->health,player->player->healthPool);
                    font_draw_text(playerHealth,FS_large,GFC_COLOR_GREEN,vector2d(275,350));
                }
                else if(player->inTown == 1)
                {
                   //slog("Town? : %i", player->inTown);
                    player->npcBeingTalkedTo->hidden = 0;
                    world_draw(town_w_1);
                    player->movementBudget_x = 100000;
                    player->movementBudget_y = 100000;
                    if(player->talkingToNpc == 1)
                    {
                        gfc_line_sprintf(playerDialougeOption1, "1. Hey there... Steve?");
                        font_draw_text(playerDialougeOption1, FS_large,GFC_COLOR_GREEN,vector2d(0,35));
                        gfc_line_sprintf(playerDialougeOption2, "2. Skibidi rizz in Ohio, snarkle steez swag god the toilet.");
                        font_draw_text(playerDialougeOption2, FS_large,GFC_COLOR_RED,vector2d(0,80));
                        
                        if(npc1->beingTalkedTo == 1)
                        {
                            gfc_line_sprintf(npcDialouge, npc1->sayTheLine);
                            font_draw_text(npcDialouge,FS_large,GFC_COLOR_WHITE,vector2d(0,0));
                        }
                        
                    }
                }
                else
                {
                    world_draw(world);
                }
                //slog("Player in Combat: %i", player->inCombat);
            
                entity_draw_all();
                
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
        
        if(player->player->health <= 0)done = 1;
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