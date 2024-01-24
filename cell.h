#ifndef _CELL_H_
#define _CELL_H_

#include <memory>
#include <vector>
#include <cstdlib>
#include "subject.h"
#include "piece.h"
using namespace std;

class Piece;

class Cell: public Subject{
    unique_ptr<Piece> p;
    int r, c; // r={nums} c={letters} range for both: 0 -> 7
    bool enpas = false;
    vector<pair<int, int>> blackThreats;
    vector<pair<int, int>> whiteThreats;

  public:
    friend class Board;
    Cell();
    Cell(int r, int c);
    Cell(Cell &&other);
    ~Cell();
    bool isEmpty() const;
    int getRow() const;
    int getCol() const;
    bool getEnpas();
    void setEnpas(bool val);
    void setPiece(Cell &other);
    void setPiece(unique_ptr<Piece> newP);
    Piece* getPiece();
    // for displays
    void notifyObservers() override;
    // threats
    void clearThreats();
    // prevents duplicates
    void addWhiteT(int row, int col);
    vector<pair<int, int>> getWhiteT();
    void addBlackT(int row, int col);
    // prevents duplicates
    vector<pair<int, int>> getBlackT();
    void notifyObservers(vector<vector<Cell>>& grid, vector<int> wKing, vector<int> bKing) override;
};

#endif
