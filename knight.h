#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "piece.h"

class Knight: public Piece {
  public:
    Knight(char colour, int r, int c);
    ~Knight();
    bool customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) override;
};

#endif
