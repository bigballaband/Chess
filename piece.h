#ifndef _PIECE_H_
#define _PIECE_H_

#include <vector>
#include "cell.h"
#include "observer.h"
#include "subject.h"
using namespace std;

class Piece : public Observer {
    pair<int, int> coords;

    char type;    // 'K' = King, 'Q' = Queen, 'R' = Rook, 'B' = Bishop, 'P' = Pawn, 'N' = Knight

    char colour;  // 'W' = white, 'B' = black

    bool firstMove; // whether piece is on its first move

    vector<pair<int, int>> captures;

    vector<Subject*> subjects;

    // custom checker for each piece
    virtual bool customChecker(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move) = 0;
  public:
    Piece(char type, char colour, int r, int c);    // ctor
    virtual ~Piece();
    char getColour() const;       // return the colour of the piece
    char getType() const;     // return the type of the piece
    bool getFirstMove() const; // is the piece on its first move
    void setFirstMove(bool val); // changes the flag
    pair<int,int> getCoords() const;
    vector<pair<int,int>> getCaptures() const;
    void setCoords(int r, int c); 
    // void resetMove(Piece *oldPiece, Piece *movedPiece, int oldRow, int oldCol, int row, int col, vector<vector<Piece>> grid);

    // determine if moving a piece form initial row and col to destRow and destCol is valid
    // NEW!! will actually perform the move instead of waiting for move method in board
    bool isValidMove(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> myKing, bool move);

    bool inBetween(int row, int col, int destRow, int destCol, int middleRow, int middleCol);

    // determines if the given piece is pinned and is NOT moveable
    // bool pin (int row, int col, int destRow, int destCol, vector<vector<Piece>> grid);

    // tells us if a piece is in between the cur cell and the destination cell
    // method will be used in the final step of verifying a valid move
    // thus we can trust that the path is either straight or diagonal
    bool blocked(int row, int col, int destRow, int destCol, 
                vector<vector<Cell>>& grid, bool diagonal=false);

    void clearCaptures();

    void addCapture(int row, int col);

    void notify(Cell &c) override {}

    // observer method
    void notify(int row, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> wKing, vector<int> bKing) override;

    void attach(Subject *s);

    void detach(Subject *s);

    // destroy method
    void notifyDestruction();
};
#endif
