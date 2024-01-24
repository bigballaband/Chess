#ifndef _PAWN_H_
#define _PAWN_H_

#include "piece.h"

class Pawn: public Piece {
  public:
   Pawn(char colour, int r, int c);
   ~Pawn();
   bool customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) override; 
};

#endif
