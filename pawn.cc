#include "pawn.h"
#include <iostream>


Pawn::Pawn(char colour, int r, int c): Piece{'P', colour, r, c} {}

Pawn::~Pawn() {}

bool Pawn::customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) {
    int threatRow, threatCol;
    // black moves normal moves
    // move 1 square
    // move 2 squares
    //   - enpas turns on
    //   - firstMove turns off
    if ((grid[row][col].getPiece()->getColour() == 'B') && (((row + 1 == destRow) && (col == destCol)))){
        if (!blocked(row, col, destRow, destCol, grid) && (grid[row+1][col].getPiece() == nullptr)){
            if (grid[myKing[0]][myKing[1]].getWhiteT().size() > 0) {
                if (grid[myKing[0]][myKing[1]].getWhiteT().size() > 1) return false;
                threatRow = grid[myKing[0]][myKing[1]].getWhiteT()[0].first;
                threatCol = grid[myKing[0]][myKing[1]].getWhiteT()[0].second;
                if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol)) { 
                    return false;
                }
            }
            if (move) {
                if (getFirstMove()) setFirstMove(false);
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
            }
            return true;
        }
    }
    else if ((grid[row][col].getPiece()->getColour() == 'B') && (row + 2 == destRow) && (col == destCol) && (grid[row][col].getPiece()->getFirstMove())) {
        if (!blocked(row, col, destRow, destCol, grid) && (grid[row + 2][col].getPiece() == nullptr)){
            if (grid[myKing[0]][myKing[1]].getWhiteT().size() > 0) {
                if (grid[myKing[0]][myKing[1]].getWhiteT().size() > 1) return false;
                threatRow = grid[myKing[0]][myKing[1]].getWhiteT()[0].first;
                threatCol = grid[myKing[0]][myKing[1]].getWhiteT()[0].second;
                if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol)) {
                    return false;
                }
            }
            // en passant set to true
            if (move) {
                grid[row][col].getPiece()->setFirstMove(false);
                grid[destRow - 1][destCol].setEnpas(true);
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
            }
            return true;
        }

    }
    
    // white moves normal moves
    // move 1 square
    // move 2 squares
    //   - enpas turns on
    //   - firstMove turns off
    else if ((grid[row][col].getPiece()->getColour() == 'W') && (((row - 1 == destRow) && (col == destCol)))){
        if (!blocked(row, col, destRow, destCol, grid) && (grid[row - 1][col].getPiece() == nullptr)){
            if (grid[myKing[0]][myKing[1]].getBlackT().size() > 0) {
                if (grid[myKing[0]][myKing[1]].getBlackT().size() > 1) return false;
                threatRow = grid[myKing[0]][myKing[1]].getBlackT()[0].first;
                threatCol = grid[myKing[0]][myKing[1]].getBlackT()[0].second;
                if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol)) {
                    return false;
                }
            } 
            if (move) {
                if (getFirstMove()) setFirstMove(false);
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
            }
            return true;
        }
    }
    else if ((grid[row][col].getPiece()->getColour() == 'W') && (row - 2 == destRow) && (col == destCol) && (grid[row][col].getPiece()->getFirstMove())) {
        if (!blocked(row, col, destRow, destCol, grid) && (grid[row - 2][col].getPiece() == nullptr)){
            if (grid[myKing[0]][myKing[1]].getBlackT().size() > 0) {
                if (grid[myKing[0]][myKing[1]].getBlackT().size() > 1) return false;
                threatRow = grid[myKing[0]][myKing[1]].getBlackT()[0].first;
                threatCol = grid[myKing[0]][myKing[1]].getBlackT()[0].second;
                if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol)) {
                    return false;
                }
            }
            // en passant set to true
            if (move) {
                grid[row][col].getPiece()->setFirstMove(false);
                grid[destRow + 1][destCol].setEnpas(true);
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
            }
            return true;
        }

    }

    // check if pawn can capture any piece (including en passant)
    // 1. check if piece exists on destination cell
    // 2. check if piece is opposite color
    // 3. OR check if cell is enpassantable
    else if ((grid[row][col].getPiece()->getColour() == 'B') && (((row + 1 == destRow) && ((col + 1 == destCol) || (col - 1 == destCol))))){
        if (grid[myKing[0]][myKing[1]].getWhiteT().size() > 0) {
            if (grid[myKing[0]][myKing[1]].getWhiteT().size() > 1) return false;
            threatRow = grid[myKing[0]][myKing[1]].getWhiteT()[0].first;
            threatCol = grid[myKing[0]][myKing[1]].getWhiteT()[0].second;
            if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol) && !(destRow == threatRow && destCol == threatCol)) {
                return false;
            }
        }
        if (!move) return true;
        if (grid[destRow][destCol].getPiece() && grid[destRow][destCol].getPiece()->getColour() == 'W') {
            if (move) {
                if (getFirstMove()) setFirstMove(false);
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
            }
            return true;
        } else if (grid[destRow][destCol].getEnpas()) {
            if (move) {
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
                grid[destRow - 1][destCol].setPiece(nullptr);
            }
            return true;
        } else {
            return false;
        }
    }
    else if ((grid[row][col].getPiece()->getColour() == 'W') && (((row - 1 == destRow) && ((col + 1 == destCol) || (col - 1 == destCol))))){
        if (grid[myKing[0]][myKing[1]].getBlackT().size() > 0) {
            if (grid[myKing[0]][myKing[1]].getBlackT().size() > 1) return false;
            threatRow = grid[myKing[0]][myKing[1]].getBlackT()[0].first;
            threatCol = grid[myKing[0]][myKing[1]].getBlackT()[0].second;
            if (!inBetween(threatRow, threatCol, myKing[0], myKing[1], destRow, destCol) && !(destRow == threatRow && destCol == threatCol)) {
                return false;
            }
        }
        if (!move) return true;
        if (grid[destRow][destCol].getPiece() && grid[destRow][destCol].getPiece()->getColour() == 'B') {
            if (move) {
                if (getFirstMove()) setFirstMove(false);
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
            }
            return true;
        } else if (grid[destRow][destCol].getEnpas()) {
            if (move) {
                grid[destRow][destCol].setPiece(grid[row][col]);
                grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
                grid[destRow + 1][destCol].setPiece(nullptr);
            }
            return true;
        } else {
            return false;
        }
    }
    else {
        return false;
    }
}
