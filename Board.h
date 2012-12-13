//Andrew Hoyle
//definition of Board class

#ifndef BOARD_H
#define	BOARD_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "MSEnum.h"
#include "Tile.h"

class Board
{
public:
    //constructor-makes Board, takes mineCount, and Board dimensions
    Board(int mc, int row, int col);
    
    //copy constructor
    Board(const Board&);
    
    //getters
    const int getRow() const {return Row;}
    const int getCol() const {return Column;}
    const int getMC() const {return mineCount;}
    const int getUCC() const {return unClickedCount;}
    const std::vector< std::vector<Tile> >& getGrid() const {return grid;}
    
    //destructor
    ~Board();
    
    //updateAllMines() calls these sub functions that look at the 
    // 8 surrounding Tiles
    //PRE: NOT an edge, updateAllMines() and algorithms() must make sure of this
    //algorithms CANNOT use this one
    int countMines(int i, int j);
    
    //algorithms can use these
    int countQuestions(int i, int j);
    int countFlags(int i, int j);
    int countUnknowns(int i, int j);
    
    //updates Tiles' values in grid(click coordinates)
    void updateAllTiles();
    
    //displays what player sees
    void displayBoard();
    
    //displays behind discovered? mask and flags and question marks
    void displayBoardCheat();
    
    //changes the current Tile, calls expandZeros() if necessary,
    // meaning if a 0 is clicked on. exp=true in this case too.
    void updateClickedTile(int& curI, int& curJ, const clickType& curCL);
    void expandZeros(int curI, int curJ);
    
    //winCheck-checks UnClickedCount, used to exit game loop in main()
    // calls updateUnClickedCount()
    void winChecker(bool& WC);
    
    //updates unClickedCount data member for wincheck
    void updateUnClickedCount();
    
    //deathCheck(clicked coordinates), used to exit game loop in main()
    void deathChecker(bool& DC, int& curI, int& curJ, const clickType& curCL);
    
private:
    int Row, Column;
    std::vector< std::vector<Tile> > grid;      //allows static array implementation
    int mineCount;
    int unClickedCount;                         //wins when 0, winChecker looks at it
};



#endif	/* BOARD_H */

