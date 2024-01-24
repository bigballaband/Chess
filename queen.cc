#include "queen.h"
#include <iostream>

Queen::Queen(char colour, int r, int c): Piece{'Q', colour, r, c} {}

Queen::~Queen() {}

bool Queen::customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) {
    int threatRow, threatCol;
    bool diagonal = true;
    // check if the move is valid or not
    if (abs(destRow - row) != abs(destCol - col)) {     // check if the move is diagonal. If not, check if the move is vertical or horizontal
        if ((row != destRow) && (col != destCol)) {     // not horizontal and vertical, i.e. both row and column are changed
            return false;
        } else {
            diagonal = false;
        }
    }
    // check if the move is blocked
    if (blocked(row, col, destRow, destCol, grid, diagonal)) {
        return false;
    } else {
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
        if (move) {    // the move is valid and move the piece
            grid[destRow][destCol].setPiece(grid[row][col]);
            grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
        }
		return true;
    }
}
