#include "cell.h"
#include "piece.h"
#include <iostream>
using namespace std;

Cell::Cell(int r, int c): Subject {}, p {nullptr}, r {r}, c {c} {}

Cell::~Cell() {}

Cell::Cell(Cell &&other) : p {std::move(other.p)}, r {other.r}, c {other.c}, enpas {other.enpas} {}

bool Cell::isEmpty() const {
    return p == nullptr;   
}

int Cell::getRow() const {
    return r;
}

int Cell::getCol() const {
    return c;
}

bool Cell::getEnpas() {
    return enpas;
}

void Cell::setEnpas(bool val) {
    enpas = val;
}

void Cell::setPiece(Cell &other) {
    p = move(other.p);
    other.notifyObservers();
    notifyObservers();
}

void Cell::setPiece(unique_ptr<Piece> newP) {
    p = move(newP);
    // cout << p->getColour() << p->getCoords().first << p->getCoords().second << p->getType() << endl;
    notifyObservers();
}

Piece* Cell::getPiece() {
    return p.get();
}

void Cell::notifyObservers() {
  for (auto ob:observers) {
    // cout << "ob" << endl;
    Piece* piece = dynamic_cast<Piece*>(ob);
    if (!piece) ob->notify(*this);
  }
}

void Cell::clearThreats() { 
    blackThreats.clear(); 
    whiteThreats.clear(); 
}

void Cell::addWhiteT(int row, int col) {
    for (const auto& threat : whiteThreats) {
        if (threat.first == row && threat.second == col) {
            return;
        }
    }
    whiteThreats.emplace_back(row, col);
}

vector<pair<int, int>> Cell::getWhiteT() {
    return whiteThreats;
}

void Cell::addBlackT(int row, int col) { 
    for (const auto& threat : blackThreats) {
        if (threat.first == row && threat.second == col) {
            return;
        }
    }
    blackThreats.emplace_back(row, col);
}

vector<pair<int, int>> Cell::getBlackT() {
    return blackThreats;
}

void Cell::notifyObservers(vector<vector<Cell>>& grid, vector<int> wKing, vector<int> bKing) {
  for (auto ob:observers) {
    Piece* piece = dynamic_cast<Piece*>(ob);
    if (piece) {
        ob->notify(r, c, piece->getCoords().first, piece->getCoords().second, grid, wKing, bKing);
    }
  }
}
