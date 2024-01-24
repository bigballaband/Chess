#include "knight.h"
#include <iostream>

Knight::Knight(char colour, int r, int c): Piece{'N', colour, r, c} {} // MIL constructor

Knight::~Knight() {}

// check for legal square to move to 
// NOTE: do not need to check for blocking square
bool Knight::customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) {
    int threatRow, threatCol;
    // check if the move is valid and move piece
    if (((col == destCol + 1) || (col == destCol - 1)) && 
        ((row == destRow + 2) || (row == destRow - 2)) ||
        ((col == destCol + 2) || (col == destCol - 2)) && 
        ((row == destRow + 1) || (row == destRow - 1))) {
        if (getColour() == 'W' && grid[myKing[0]][myKing[1]].getBlackT().size() > 0) {
            if (grid[myKing[0]][myKing[1]].getBlackT().size() > 1) return false;
            threatRow = grid[myKing[0]][myKing[1]].getBlackT()[0].first;
            threatCol = grid[myKing[0]][myKing[1]].getBlackT()[0].second;
            if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol) && !(destRow == threatRow && destCol == threatCol)) {
                return false;
            }
        } else if (getColour() == 'B' && grid[myKing[0]][myKing[1]].getWhiteT().size() > 0) {
            if (grid[myKing[0]][myKing[1]].getWhiteT().size() > 1) return false;
            threatRow = grid[myKing[0]][myKing[1]].getWhiteT()[0].first;
            threatCol = grid[myKing[0]][myKing[1]].getWhiteT()[0].second;
            if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol) && !(destRow == threatRow && destCol == threatCol)) {
                return false;
            }
        }
        if (move) {
            grid[destRow][destCol].setPiece(grid[row][col]);
            grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
        }
        return true;
    }
    else {
        return false;
    }
}

