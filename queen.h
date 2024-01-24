#ifndef _QUEEN_H_
#define _QUEEN_H_

#include "piece.h"

class Queen: public Piece {
  public:
    Queen(char colour, int r, int c);
    ~Queen();
    bool customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) override;
};

#endif
