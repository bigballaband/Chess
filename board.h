#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdlib>

#include "cell.h"
#include "graphicsdisplay.h"
#include "textdisplay.h"
#include "piece.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "king.h"
#include "pawn.h"

class Board : public Observer {
    int W = 0;     // 0: human (default), 1: lvl 1 bot, 2: lvl 2 bot, 3: lvl 3 bot, 4: lvl 4+ bot
    int B = 0;
    vector<vector<Cell>> grid;
    char turn = 'W';
    int scoreW = 0;
    int scoreB = 0;
    bool wKingCheck = false;
    bool bKingCheck = false;
    //bool gameRunning = false;
    bool setUpDone = false;
    vector<int> bKingCoord = {0,0};
    vector<int> wKingCoord = {0,0};
    vector<int> enpasCoord = {0,0};
    vector<pair<int, int>> wPieceCoords;
    vector<pair<int, int>> bPieceCoords;
    unique_ptr<TextDisplay> td;
    unique_ptr<GraphicsDisplay> gd;
    Xwindow &xw;
    // need work
    bool pin(int row, int col, int destRow, int destCol);
    void inCheck(char pieceColour);
    vector<pair<int,int>> coordsOfMove();
    bool moveable();
    void changeTurn();
    bool checkEnd();
    void move(string start, string dest, char promote='P');
    void botMove1();
    void botMove2();
    void botMove3();
    // void botMove4();
    void notify(Cell &c);
  public:
    Board(Xwindow &xw);
    ~Board();
    void runGame(string player1, string player2);
    void reset();
    void setUp();
    int getWScore();
    int getBScore();
};

#endif
