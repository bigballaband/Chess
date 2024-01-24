#include "piece.h"
#include "cell.h"
#include <iostream>
Piece::Piece(char type, char colour, int r, int c): coords {r, c}, type {type}, colour{colour}, firstMove {true} {}

Piece::~Piece() { notifyDestruction(); }

char Piece::getColour() const { return colour; }

char Piece::getType() const { return type; }

bool Piece::getFirstMove() const { return firstMove; }

void Piece::setFirstMove(bool val) { firstMove = val; }

pair<int,int> Piece::getCoords() const {return coords;}
void Piece::setCoords(int r, int c) {coords = make_pair(r, c);}

vector<pair<int,int>> Piece::getCaptures() const {return captures;}

bool Piece::isValidMove(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) {
  // check for edge cases:
  // 1. empty cell
  // 2. capturing same color
  // cout << row << col << destRow << destCol << endl;

  // cout << "move " << row << ", " << col << " to " << destRow << ", " << destCol << endl;

  if (grid[destRow][destCol].getPiece() && colour == grid[destRow][destCol].getPiece()->getColour() && move) {
    // run checker that is custom to each piece
    
    return false;
  } else {
    // cout << "valid" << endl;
    return customChecker(row, col, destRow, destCol, grid, myKing, move);
  }
}

bool Piece::inBetween(int row, int col, int destRow, int destCol, int middleRow, int middleCol) {
  // Check for horizontal path
  if (row == destRow) {
      return (middleRow == row) && (std::min(col, destCol) < middleCol) && (middleCol < std::max(col, destCol));
  }
  // Check for vertical path
  if (col == destCol) {
      return (middleCol == col) && (std::min(row, destRow) < middleRow) && (middleRow < std::max(row, destRow));
  }
  // Check for diagonal path
  if (std::abs(row - destRow) == std::abs(col - destCol)) {
      bool isMiddleOnDiagonal = std::abs(middleRow - row) == std::abs(middleCol - col) && 
                                std::abs(middleRow - destRow) == std::abs(middleCol - destCol);
      return isMiddleOnDiagonal && 
              (((std::min(row, destRow) < middleRow) && (middleRow < std::max(row, destRow))) || 
              ((std::min(col, destCol) < middleCol) && (middleCol < std::max(col, destCol))));
  }
  return false;
}

bool Piece::blocked(int row, int col, int destRow, int destCol, 
                    vector<vector<Cell>>& grid, bool diagonal) {
  int j = row;
  int xDiff = col - destCol;
  int yDiff = row - destRow;
  // cout << col << row << destCol << destRow << endl;
  if (diagonal) {
    if (xDiff < 0) {
      for (int i = col + 1; i < destCol; ++i) {
        if (yDiff > 0) --j;
        else ++j;
        if (grid[j][i].getPiece() && grid[j][i].getPiece()->getType() != 'K') {
          return true;
        }
      }
    } else if (xDiff > 0) {
      for (int i = col - 1; i > destCol; --i) {
        if (yDiff > 0) --j;
        else ++j;
        if (grid[j][i].getPiece() && grid[j][i].getPiece()->getType() != 'K') {
          return true;
        }
      }
    }
  } else {
    if (yDiff < 0) {
      for (int i = row + 1; i < destRow; ++i) {
        if (grid[i][col].getPiece() && grid[i][col].getPiece()->getType() != 'K') return true;
      }
    } else if (yDiff > 0) {
      for (int i = row - 1; i > destRow; --i) {
        if (grid[i][col].getPiece() && grid[i][col].getPiece()->getType() != 'K') return true;
      }
    } else if (xDiff < 0) {
      for (int i = col + 1; i < destCol; ++i) {
        if (grid[row][i].getPiece() && grid[row][i].getPiece()->getType() != 'K') return true;
      }
    } else if (xDiff > 0) {
      for (int i = col - 1; i > destCol; --i) {
        if (grid[row][i].getPiece() && grid[row][i].getPiece()->getType() != 'K') return true;
      }
    }
  } 

  return false;
}

void Piece::clearCaptures() { captures.clear(); }

void Piece::addCapture(int row, int col) { captures.emplace_back(row, col); }

// observer method
void Piece::notify(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> wKing, vector<int> bKing) {
    // same cell
    if (row == destRow && col == destCol) return;
    // otherwise
    //if (grid[row][col].getPiece() && grid[row][col].getPiece()->isValidMove(row, col, destRow, destCol, grid, false)){
      //cout << "adding a piece" << endl;
       // grid[row][col].getPiece()->addCapture(destRow, destCol);  
   // } 
   if (!grid[destRow][destCol].getPiece()) return;
    if (grid[destRow][destCol].getPiece()->getColour() == 'W' && (isValidMove(destRow, destCol, row, col, grid, wKing, false))) {
      if (grid[destRow][destCol].getPiece()->getType() == 'P') {
        if (col != destCol) {
          grid[row][col].addWhiteT(destRow, destCol);
          if (grid[row][col].getPiece() && grid[row][col].getPiece()->getColour() == 'B') grid[destRow][destCol].getPiece()->addCapture(row, col);
        } else {
          grid[destRow][destCol].getPiece()->addCapture(row, col);
        }
      }
      else {
        grid[row][col].addWhiteT(destRow, destCol);
        if (!grid[row][col].getPiece() || grid[row][col].getPiece()->getColour() == 'B') {
          if ((grid[destRow][destCol].getPiece()->getType() == 'K' && 
              ((getColour() == 'W' && (grid[row][col].getBlackT().size() == 0)))) ||
              grid[destRow][destCol].getPiece()->getType() != 'K') {
            grid[destRow][destCol].getPiece()->addCapture(row, col);
          } 
        }
      }
    } else if (grid[destRow][destCol].getPiece()->getColour() == 'B' && isValidMove(destRow, destCol, row, col, grid, bKing, false)) {
      if (grid[destRow][destCol].getPiece()->getType() == 'P') {
        if (col != destCol) {
          grid[row][col].addBlackT(destRow, destCol);
          if (grid[row][col].getPiece() && grid[row][col].getPiece()->getColour() == 'W') grid[destRow][destCol].getPiece()->addCapture(row, col);
        } else {
          grid[destRow][destCol].getPiece()->addCapture(row, col);
        }
      }
      else {
        grid[row][col].addBlackT(destRow, destCol);
        if (!grid[row][col].getPiece() || grid[row][col].getPiece()->getColour() == 'W') {
          if ((grid[destRow][destCol].getPiece()->getType() == 'K' && 
			        (getColour() == 'B' && (grid[row][col].getWhiteT().size() == 0))) ||
              grid[destRow][destCol].getPiece()->getType() != 'K') {
            grid[destRow][destCol].getPiece()->addCapture(row, col);
          } 
        }
      }
    }
}

void Piece::attach(Subject *s) {
  subjects.emplace_back(s);
}

void Piece::detach(Subject *s) {
  for (auto it = subjects.begin(); it != subjects.end();) {
    if (*it == s) {
      it = subjects.erase(it);
      break;
    } else {
      ++it;
    }
  }
}

// destroy method
void Piece::notifyDestruction() {
  for (auto s:subjects) {
    s->detach(this);
  }
}
