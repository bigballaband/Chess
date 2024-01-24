#include "king.h"
#include <iostream>

King::King(char colour, int r, int c): Piece{'K', colour, r, c} {}

King::~King() {}

// TODO: check if king is in check
// TODO: king can't pass through check
bool King::customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) {
	int colDiff = destCol - col;
  int rowDiff = destRow - row;

	Piece *possibleRook1 = grid[row][0].getPiece();
	Piece *possibleRook2 = grid[row][7].getPiece();

	// check for castling
	// NOTE: we will have to check that it is a castle in the move method from board
	// 1. check if positions are valid
	// 2. check if rook and king are unmoved
	// 3. check if any pieces in the way
	// 4. king and rook are no longer on their first moves

	// TODO: check for castling through square that are in threat
	if (rowDiff == 0 && getFirstMove()) {
		if (possibleRook1 && possibleRook1->getType() == 'R' && 
				possibleRook1->getFirstMove() && colDiff == -2 &&
				!blocked(row, col, destRow, destCol, grid) &&
				((row == 0 && grid[row][col - 1].getWhiteT().size() == 0 && 
					grid[row][col - 2].getWhiteT().size() == 0) ||
				 (row == 7 && grid[row][col - 1].getBlackT().size() == 0 && 
					grid[row][col - 2].getBlackT().size() == 0))) {
			if (move) {
				grid[row][destCol].setPiece(grid[row][col]);
				grid[row][destCol].getPiece()->setCoords(row, col);
				grid[row][col - 1].setPiece(grid[row][0]);
				grid[row][col - 1].getPiece()->setCoords(row, col - 1);
				setFirstMove(false);
				possibleRook1->setFirstMove(false);
			}
			return true;
		}
		if (possibleRook2 && possibleRook2->getType() == 'R' && 
				possibleRook2->getFirstMove() && colDiff == 2 &&
				!blocked(row, col, destRow, destCol, grid) && 
				((row == 0 && grid[row][col + 1].getWhiteT().size() == 0 && 
					grid[row][col + 2].getWhiteT().size() == 0) ||
				 (row == 7 && grid[row][col + 1].getBlackT().size() == 0 && 
					grid[row][col + 2].getBlackT().size() == 0))) {
			if (move) {
				grid[row][destCol].setPiece(grid[row][col]);
				grid[row][destCol].getPiece()->setCoords(row, destCol);
				grid[row][col + 1].setPiece(grid[row][7]);
				grid[row][col + 1].getPiece()->setCoords(row, col + 1);
				setFirstMove(false);
				possibleRook2->setFirstMove(false);
			}
			return true;
			}
		}

	// regular move
	// TODO: destination square is in threat
	if (((colDiff == 0 && (rowDiff == 1 || rowDiff == -1)) || 
			 (rowDiff == 0 && (colDiff == 1 || colDiff == -1)) || 
			 ((rowDiff == 1 || rowDiff == -1) && (colDiff == 1 || colDiff == -1))) && 
			 (!move || ((getColour() == 'W' && (grid[destRow][destCol].getBlackT().size() == 0)) ||
			 (getColour() == 'B' && (grid[destRow][destCol].getWhiteT().size() == 0))))) {
		if (move) { 
			if (getFirstMove()) setFirstMove(false);
			grid[destRow][destCol].setPiece(grid[row][col]);
			grid[destRow][destCol].getPiece()->setCoords(destRow, destCol);
		}
		return true;
	}

	return false;
}