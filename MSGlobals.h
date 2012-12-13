/* 
 * File:   MSGlobals.h
 * Author: mettledrum
 *
 * Created on December 12, 2012, 4:12 PM
 */

#ifndef MSGLOBALS_H
#define	MSGLOBALS_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <cstdlib>
#include <SDL/SDL_events.h>

//GLOBALS for main() and subfunctions -------------externs----------------------

//The surfaces
extern SDL_Surface *screen;             //where tiles reside
extern SDL_Surface *tileSheet;          //place for tiles under the hood
extern SDL_Surface *menuPic;

extern SDL_Surface *boardPic1;          //16x16 tiles, 240x240 pix
extern SDL_Surface *winPic1;
extern SDL_Surface *losePic1;

extern SDL_Surface *boardPic2;           //25x25, 375x375
extern SDL_Surface *winPic2;
extern SDL_Surface *losePic2;

extern SDL_Surface *boardPic3;          //33x33, 495x495
extern SDL_Surface *winPic3;
extern SDL_Surface *losePic3;

//for mouse clicks, numbers, and quit choices
extern SDL_Event event;

//Tile constants
extern const int TILE_WIDTH;              //heigh==width here
extern const int TILE_HEIGHT;
extern const int TILE_SPRITES;            //number in clips[], (different tiles)

//sections from tileSheet for all possible tile values
extern SDL_Rect clips[];

//------------------------------------------externs-----------------------------

#endif	/* MSGLOBALS_H */

