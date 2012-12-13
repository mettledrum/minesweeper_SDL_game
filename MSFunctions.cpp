//Andrew Hoyle

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"              //IMG_Load()
#include <string>
#include <cstdlib>
#include <SDL/SDL_events.h>

#include "MSGlobals.h"

#include "Board.h"
#include "Tile.h"                       //clean_up()
#include "MSFunctions.h"
#include "MSEnum.h"

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    // needs -lSDL_image extension//////////////////////////////////////////////
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized surface
    return optimizedImage;
}

//change x, y to SDL_Rect? /////////////////////////////////////////////////////
void apply_surface( int x, int y, 
        SDL_Surface* source, 
        SDL_Surface* destination, 
        SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface(source, clip, destination, &offset);
}

bool init(int w, int h, int bpp, SDL_Surface** surf)
{  
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

    //Set up the screen
    *surf = SDL_SetVideoMode( w, h, bpp, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( *surf == NULL )
        return false;

    //Set the window caption
    SDL_WM_SetCaption("Minesweeper Weird Ed.", NULL);

    //If everything initialized fine
    return true;
}

bool load_file(SDL_Surface** surf, std::string surfName)
{
    *surf = load_image(surfName);
    
    if(*surf == NULL)
        return false;
    
    return true;
}

//returns idx for Board grid
//inherent floor function in int division
int coord_to_idx(int coord, int spriteLength)
{
    int idx=0;
    
    while(coord>=spriteLength)
    {
        ++idx;
        coord-=spriteLength;
    }
    
    return idx;
}

//used to show grid
int get_tile_val(const Tile& T)
{
    if(T.edge==true)
        return 9;
    else if (T.discovered==false)
    {
        if(T.flag==true)
            return 10;
        else if(T.question==true)
            return 12;
        else
            return 14;
    }
    else
        return T.adjMines;
}

//shows at end of game
// wrong flags are Xs
// correct flags are flags
// mines are mines
// numbers are numbers
int get_tile_val_end(const Tile& T)
{
    if(T.edge==true)
        return 9;               //edge
    else if(T.flag==true)
    {
        if(T.mine==false)
            return 13;          //wrong flag
        else
            return 10;
    }
    else if(T.mine==true)
        return 11;              //mine
    else
        return T.adjMines;
}

//sprite sheet had better be the right dimension!
//15 are made
void format_clips(SDL_Rect clips[], int clipSize, int spriteHeight)
{
    for(int i=0; i<clipSize; ++i)
    {
        clips[i].x=0;
        clips[i].y=i*spriteHeight;
        clips[i].w=spriteHeight;
        clips[i].h=spriteHeight;
    }
}

void show_grid(const Board& b, 
        SDL_Rect cl[], 
        int clSize, 
        SDL_Surface* source, 
        SDL_Surface* dest,
        const int spriteSize,
        bool stillPlaying)
{
    //between 0-255
    int tileOpacity = 190;
    
    for(int i=0; i<b.getRow(); ++i)
    {
        for(int j=0; j<b.getRow(); ++j)
        {
            SDL_Rect offset;
            offset.x = j*spriteSize;
            offset.y = i*spriteSize;
            
            //for displaying clips
            int tempClipValue;
            
            //get values for tiles based on if the game
            // is over or not
            if(stillPlaying==true)
                tempClipValue = get_tile_val(b.getGrid()[i][j]);
            else
                tempClipValue = get_tile_val_end(b.getGrid()[i][j]);
            
            //for alpha transparency for uncovered tiles
            if(tempClipValue == 14)
                SDL_SetAlpha(source, SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
            else if(tempClipValue == 9)
                SDL_SetAlpha(source, SDL_SRCALPHA, tileOpacity);
            else
                SDL_SetAlpha(source, SDL_SRCALPHA, tileOpacity);
            
            SDL_BlitSurface(source, &cl[tempClipValue], dest, &offset);
        }
    }
}

void play_game(Board& B, bool& Q, bool& W, SDL_Surface* boardPic)
{
    //for flag and clicks
    clickType curClick;

    //ending the current game
    bool dead = false;
    bool won = false;
    bool stillPlaying;
    
    //reset main() values
    Q=false;
    W=false;
    
    //playing the game loop, displays game
    // updates Q, W and returns if user quits,
    // dies or wins.
    //if user win/dies, final screen is printed
    // before returning to main()
    while(dead == false && won == false) 
    {
        //event queue
        while( SDL_PollEvent( &event ) )
        {
            if(event.type == SDL_MOUSEBUTTONUP)
            {
                int tempX, tempY;
                tempX = coord_to_idx(event.button.y, TILE_WIDTH);
                tempY = coord_to_idx(event.button.x, TILE_HEIGHT);

                //includes "wrappers" for Board class correct input
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    //set to click enum
                    curClick=click;

                    //check won/dead
                    B.deathChecker(dead, tempX, tempY, curClick);

                    //jump out of play_game() if true
                    if(dead==true || won==true)
                    {
                        continue;
                    }
                }
                //right click
                else
                    curClick=flag;

                //update function handles all tile values' changes,
                // don't allow border to be clicked
                if(tempX!=0 && tempY!=0 && 
                        tempY!= B.getRow()-1 && tempX!=B.getCol()-1)
                    B.updateClickedTile(tempX, tempY, curClick);

                //board updated
                B.updateAllTiles();

                //check if uncovered MC == 0
                B.winChecker(won);

                //exit game if true
                if(won==true || dead==true)
                    continue;
            }

            //top right X clicked, change Q by reference, return to main()
            if( event.type == SDL_QUIT )
            {
                Q=true;
                return;
            }
        }

        //show background pic on bottom "layer"
        SDL_BlitSurface(boardPic, NULL, screen, NULL); 

        //helps determine which board is shown,
        // whether it's the end of the game
        stillPlaying = !(dead || won);

        //uses Blit, displays end pics/tiles 
        // if dead or won, or still playing
        show_grid(B, clips, TILE_SPRITES, tileSheet, screen, TILE_WIDTH, stillPlaying);

        //actually show the blitted surfaces
        if( SDL_Flip( screen ) == -1 )
            exit(3);
        
        //return if dead or won, keep looping if not
        if(dead==true || won==true)
        {
            if(won==true)
                W=true;
            
            return;
        }
    }
}

void display_final(Board& B, bool& winner, SDL_Surface* winPic, SDL_Surface* losePic)
{
    bool stillPlaying=false;
    
    //change background pic
    if(winner==true)
        SDL_BlitSurface(winPic, NULL, screen, NULL);
    else
        SDL_BlitSurface(losePic, NULL, screen, NULL);

    //show final grid
    show_grid(B, clips, TILE_SPRITES, tileSheet, screen, TILE_WIDTH, stillPlaying);

    //show the blitted surfaces
    if( SDL_Flip( screen ) == -1 )
        exit(3);
}

void wait_123Q(int& l_s, bool& qqq)
{
    //only happens first game b/c l_s value!=0 
    // after first time
    while(l_s==0 && qqq==false)
    {
        while( SDL_PollEvent( &event ) )
        {
            //quit
            if( event.type == SDL_QUIT )
            {
                qqq=true;
                continue;
            }

            //key pressed, can start over this way
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_1)
                    l_s=1;
                else if(event.key.keysym.sym == SDLK_2)
                    l_s=2;
                else if(event.key.keysym.sym == SDLK_3)
                    l_s=3;
                else {}
            }
        }
    }
}

void clean_up()
{
    //game has been QUIT, cleanup stuff to prevent mem leak
    SDL_FreeSurface(tileSheet);
    SDL_FreeSurface(menuPic);
    
    SDL_FreeSurface(boardPic1);
    SDL_FreeSurface(losePic1);
    SDL_FreeSurface(winPic1);
    
    SDL_FreeSurface(boardPic2);
    SDL_FreeSurface(losePic2);
    SDL_FreeSurface(winPic2);
    
    SDL_FreeSurface(boardPic3);
    SDL_FreeSurface(losePic3);
    SDL_FreeSurface(winPic3);
}

void load_rectangles()
{
    //Load all the pictures for tiles
    if( load_file( &tileSheet, "mine_pics/tiles.png" )          == false ||
        load_file( &menuPic, "mine_pics/menu_window.png" )      == false ||
        load_file( &boardPic1, "mine_pics/board1.jpg" )         == false ||
        load_file( &winPic1, "mine_pics/win1.png" )             == false ||
        load_file( &losePic1, "mine_pics/lose1.png" )           == false ||
        load_file( &boardPic2, "mine_pics/board2.jpg" )         == false ||
        load_file( &winPic2, "mine_pics/win2.jpg" )             == false ||
        load_file( &losePic2, "mine_pics/lose2.jpg" )           == false ||
        load_file( &boardPic3, "mine_pics/board3.jpg" )         == false ||
        load_file( &winPic3, "mine_pics/win3.jpg" )             == false ||
        load_file( &losePic3, "mine_pics/lose3.jpg" )           == false )
        exit(2);    
}

