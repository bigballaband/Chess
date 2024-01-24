#include "bishop.h"
#include <cstdlib>
#include <iostream>
using namespace std;

Bishop::Bishop(char colour, int r, int c): Piece{'B', colour, r, c} {}

Bishop::~Bishop() {}

bool Bishop::customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) {
    // check if the move is diagonal, i.e. the change in number of rows and columns should be the same
    int threatRow, threatCol;
    if (abs(destRow - row) != abs(destCol - col)) {
        return false;
    }
    // check if the move is blocked
    if (blocked(row, col, destRow, destCol, grid, true)) {
        return false;
    } else if (getColour() == 'W' && grid[myKing[0]][myKing[1]].getBlackT().size() > 0) {
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
