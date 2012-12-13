//Andrew Hoyle

#ifndef TILE_H
#define	TILE_H

//forward declaration for friendship
class Board;

class Tile
{
public:
    //makes board copy constructor easier to implement
    Tile& operator=(const Tile& t);

    //so Board can get private data members,
    //  and no other functions can
    friend class Board;

//private:
    //around the grid
    bool edge;

    //can change with "click"
    bool question;
    bool flag;

    bool mine;
    bool discovered;

    //populated by the board class in its constructor
    int adjMines;

    //used by expandZeros() in board class for recursion
    bool visited;

    //updated by the board class
    int adjUnknowns;
    int adjFlags;
    int adjQuestions;
};


#endif	/* TILE_H */

