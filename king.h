#ifndef _KING_H_
#define _KING_H_

#include "piece.h"

class King: public Piece {
  public:
    King(char colour, int r, int c);
    ~King();
    bool customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) override; 
};

#endif
