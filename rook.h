#ifndef _ROOK_H_
#define _ROOK_H_

#include "piece.h"

class Rook: public Piece {
  public:
    Rook(char colour, int r, int c);
    ~Rook();
    bool customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) override;
};

#endif