//Andrew Hoyle

/*NOTES:
 * 11/20/12
 *      -no globals, surfaces aren't getting changed! USED SDL_SURFACE**
 *      -while() doesn't need to constantly refresh unless yellow highlighting
 *      -top layer is to be blitted last... so Earth before tiles
 *      -make so flag cannot be clicked on
 * 
 * 12/12/12
 *      -globals now, figured out extern keyword
 *      -main() - controls levels displayed, prints win/lose screen
 *      -play_game() - changes WIN/QUIT bools BY REFERENCE, so main() can 
 *       use them for displaying purposes
 *      -play_game() and main() ALWAYS listen for quit
 *      -only main() listens for 1,2, 3 values
 *      -has NOT been tested with non-square boards
 *      -extreme props to Lazy Foo' Productions!        http://lazyfoo.net/
 */

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <SDL/SDL_events.h>

#include "MSGlobals.h"

#include "MSEnum.h"
#include "Tile.h"
#include "MSFunctions.h"        
#include "Board.h"

//GLOBALS for main() and subfunctions -------------externs----------------------

//The surfaces
SDL_Surface *screen = NULL;             //where tiles reside
SDL_Surface *tileSheet = NULL;          //place for tiles under the hood
SDL_Surface *menuPic = NULL;

SDL_Surface *boardPic1 = NULL;          //16x16 tiles, 240x240 pix
SDL_Surface *winPic1 = NULL;
SDL_Surface *losePic1 = NULL;

SDL_Surface *boardPic2 = NULL;           //25x25, 375x375
SDL_Surface *winPic2 = NULL;
SDL_Surface *losePic2 = NULL;

SDL_Surface *boardPic3 = NULL;          //33x33, 495x495
SDL_Surface *winPic3 = NULL;
SDL_Surface *losePic3 = NULL;

//for mouse clicks, numbers, and quit choices
SDL_Event event;

//Tile constants
const int TILE_WIDTH = 15;              //heigh==width here
const int TILE_HEIGHT = 15;
const int TILE_SPRITES = 15;            //number in clips[], (different tiles)

//sections from tileSheet for all possible tile values
SDL_Rect clips[ TILE_SPRITES ];

//------------------------------------------externs-----------------------------

//main() "GLOBALS"--------------------------------------------------------------

//screen values, must be same for Board Dimensions
// b/c of coord_to_idx()
const int SCREEN_BPP = 32;

//just for me for reference for tileSheet file:
//const int EDGE = 9;
//const int FLAG = 10;
//const int MINE = 11;
//const int QUESTION = 12;
//const int NOTMINE = 13;
//const int COVERED = 14;                 //made transparent from show_grid()

//------------------------------------------------------------------------------

int main() 
{
    //new main "GLOBALS" for restart/start control loop
    // play_game() refreshes QUIT and WIN by ref&
    int level_sel=0;
    bool QUIT=false;
    bool WIN=false;
    
    //Initialize screen for 123menu, 240x240px
    if(init(240, 240, SCREEN_BPP, &screen)==false)
        return 1;

    //uses global SDL_Surfaces, loads them, 
    // exits(2) if error loading any
    load_rectangles();
    
    //chop up tiles.png into TILE_SPRITES many 15x15 pieces (15)
    format_clips(clips, TILE_SPRITES, TILE_HEIGHT);
    
    //start/restart here while game hasn't quit
    while(QUIT==false)
    {
        //show menu just first time
        if(level_sel==0)
        {
            SDL_BlitSurface(menuPic, NULL, screen, NULL);
            if( SDL_Flip( screen ) == -1 )
                return 3;
        }
        
        //gets user input using GLOBAL events first time only
        wait_123Q(level_sel, QUIT);
        
        //leaves game before showing anything else
        if(QUIT == true)
            continue;        
     
        //play ball based on 123
        if(level_sel == 1)
        {
            //small screen
            if(init(240, 240, SCREEN_BPP, &screen)==false)
                return 1;
            
            //make board based on dimensions,
            // pass board to play_game() by ref&
            Board B(14, 16, 16);
            play_game(B, QUIT, WIN, boardPic1);
            
            //update, destruct B
            if(QUIT == false)
                display_final(B, WIN, winPic1, losePic1);
        }
        else if(level_sel == 2)
        {
            //medium screen
            if(init(375, 375, SCREEN_BPP, &screen)==false)
                return 1;            
            
            //same thing, different dimensions
            Board B(52, 25, 25);
            play_game(B, QUIT, WIN, boardPic2);    
            
            //update, destruct B
            if(QUIT == false)
                display_final(B, WIN, winPic2, losePic2);
        }
        else
        {
            //large screen
            if(init(495, 495, SCREEN_BPP, &screen)==false)
                return 1;            
            
            //third, hardest dimensions
            Board B(150, 33, 33);
            play_game(B, QUIT, WIN, boardPic3);
            
            //update, destruct B
            if(QUIT == false)
                display_final(B, WIN, winPic3, losePic3);
        }
        
        //leaves game before showing anything else
        if(QUIT == true)
            continue;        
        
        //end screen has been displayed, now wait for 1,2,3, or quit
        level_sel = 0;
        //gets user input using GLOBAL events
        wait_123Q(level_sel, QUIT);
    }
    
    //QUIT==true, end game, prevent mem. leak
    clean_up();
    SDL_Quit();

    return 0;
}

