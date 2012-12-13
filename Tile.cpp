//Andrew Hoyle

#include "Tile.h"


//makes board copy constructor easier to implement
Tile& Tile::operator=(const Tile& t)
{
    edge=t.edge;
    question=t.question;
    flag=t.flag;
    mine=t.mine;
    discovered=t.discovered;
    adjMines=t.adjMines;
    visited=t.visited;
    adjUnknowns=t.adjUnknowns;
    adjFlags=t.adjFlags;
    adjQuestions=t.adjQuestions;

    return *this;
}