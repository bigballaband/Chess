#include "rook.h"
#include <iostream>

Rook::Rook(char colour, int r, int c): Piece{'R', colour, r, c} {}

Rook::~Rook() {}


bool Rook::customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) {
    int threatRow, threatCol;
    // both row and column has changed
    if ((row != destRow && col != destCol) || blocked(row,col,destRow,destCol,grid)) {
        return false;
    }
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
