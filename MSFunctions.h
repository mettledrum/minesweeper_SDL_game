//Andrew Hoyle

#ifndef MSFUNCTIONS_H
#define	MSFUNCTIONS_H

#include "SDL/SDL.h"
#include <string>

#include "Board.h"

SDL_Surface *load_image( std::string filename );
bool init(int w, int h, int bpp, SDL_Surface** surf);
bool load_file(SDL_Surface** surf, std::string surfName);

void apply_surface( int x, int y, 
        SDL_Surface* source, 
        SDL_Surface* destination, 
        SDL_Rect* clip);

int coord_to_idx(int coord, int spriteLength);

void format_clips(SDL_Rect clips[], int clipSize, int spriteHeight);

int get_tile_val(const Tile& T);
int get_tile_val_end(const Tile& T);

void show_grid(const Board& b, 
        SDL_Rect cl[], 
        int clSize, 
        SDL_Surface* source, 
        SDL_Surface* dest,
        const int spriteSize,
        bool stillPlaying);

void play_game(Board& B, bool& Q, bool& W, SDL_Surface* boardPic);

void display_final(Board& B, bool& winner, SDL_Surface* winPic, SDL_Surface* losePic);

void wait_123Q(int& l_s, bool& qqq);

void clean_up();

void load_rectangles();

#endif	/* MSFUNCTIONS_H */

