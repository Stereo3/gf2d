#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "font.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "world.h"
#include "enemy.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    World *world;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    Player *player;
    Entity *pirateShip1;
    TextLine fps, player_pos, movementBudgets;
    
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
    
    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    player = player_new("greg");
    world = world_load("maps/testworld.map");
    pirateShip1 = enemy_new(vector2d(96,96));
    world_setup_camera(world);
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
        
        entity_think_all();
        entity_update_all();
        player_think(player);
        player_update(player);
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first            
            world_draw(world);
        
            entity_draw_all();
            
            if(keys[SDL_SCANCODE_TAB])
            {
                gfc_line_sprintf(player_pos,"Players Position| X: %f, Y: %f", player->player->position.x, player->player->position.y);
                font_draw_text(player_pos,FS_small, GFC_COLOR_CYAN,vector2d(10,10));
                gfc_line_sprintf(fps,"FPS: %f",gf2d_graphics_get_frames_per_second());
                font_draw_text(fps,FS_small,GFC_COLOR_GREEN,vector2d(10,40));
                gfc_line_sprintf(movementBudgets,"Movement Budgets | X: %f, Y: %f",player->movementBudget_x, player->movementBudget_y);
                font_draw_text(movementBudgets,FS_small,GFC_COLOR_RED,vector2d(10,70));
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
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    player_free(player);
    world_free(world);
    slog("---==== END %s ====---", argv[0]);
    return 0;
}
/*eol@eof*/