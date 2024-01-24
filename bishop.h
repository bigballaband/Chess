#ifndef _BISHOP_H_
#define _BISHOP_H_

#include "piece.h"

class Bishop: public Piece {
  public:
    Bishop(char colour, int r, int c);
    ~Bishop();
    bool customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) override;
};

#endif
