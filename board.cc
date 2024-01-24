#include "board.h"
using namespace std;

void attachObserverToCell(int row, int col, vector<vector<Cell>> &grid) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j].getPiece()) {
                grid[row][col].attach(grid[i][j].getPiece());
                grid[i][j].getPiece()->attach(&(grid[row][col]));
            }
        }
    }
}


Board::Board(Xwindow &xw): W {0}, B {0}, grid {}, turn {'W'}, scoreW {0}, scoreB {0},td{nullptr},gd{nullptr}, bKingCoord{0,4}, wKingCoord{7,4}, xw {xw} {
    td = make_unique<TextDisplay>();
    gd = make_unique<GraphicsDisplay>(xw);

    // placing empty cells in grid
    for (int i = 0; i < 8; ++i) {
        vector<Cell> row;
        for (int j = 0; j < 8; ++j) {
            Cell c {i, j};
            row.emplace_back(std::move(c));
            
        }
        grid.emplace_back(std::move(row));
    }
    // adding listeners
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            grid[i][j].attach(td.get());
            grid[i][j].attach(gd.get());
            grid[i][j].attach(this);
            
        }
    }
    
}

Board::~Board() {}

void Board::inCheck(char pieceColour) {
    int kingCoordR;
    int kingCoordC;
    if (pieceColour == 'W') {
        kingCoordR = wKingCoord[0];
        kingCoordC = wKingCoord[1];
        // cout << grid[kingCoordR][kingCoordC].getBlackT().size() << endl; 
        if (grid[kingCoordR][kingCoordC].getBlackT().size() > 0) {
            wKingCheck = true;
        } else {
            wKingCheck = false; // resets the king's check flag
        }
    } else {
        kingCoordR = bKingCoord[0];
        kingCoordC = bKingCoord[1];
        // cout << grid[kingCoordR][kingCoordC].getWhiteT().size() << endl;  
        if (grid[kingCoordR][kingCoordC].getWhiteT().size() > 0) {
            bKingCheck = true;
        } else {
            bKingCheck = false; // resets the king's check flag
        }
    }
}


bool Board::moveable() {
    if (turn == 'W'){
        for (auto i: wPieceCoords){
            if (!grid[i.first][i.second].getPiece()->getCaptures().empty()){
                for (auto j: grid[i.first][i.second].getPiece()->getCaptures()){
                    if (grid[i.first][i.second].getPiece()->getType() == 'K' || !(pin(i.first, i.second, j.first, j.second))) {
                        return true;
                    }
                }
            }
        }
        return false;
    } else {
        for (auto i: bPieceCoords){
            if (!grid[i.first][i.second].getPiece()->getCaptures().empty()){
                for (auto j: grid[i.first][i.second].getPiece()->getCaptures()){
                    if (grid[i.first][i.second].getPiece()->getType() == 'K' || !(pin(i.first, i.second, j.first, j.second))) {
                        return true;
                    }
                }
            } 
        }  
        return false;
    }
}

bool Board::checkEnd() {
    inCheck(turn);
    if (!moveable()){
        if (wKingCheck){
            cout << "Checkmate! ";
            ++scoreB;
            cout << "Black wins!" << endl;
        } else if (bKingCheck) {
            cout << "Checkmate! ";
            ++scoreW;
            cout << "White wins!" << endl;
        } else {
            cout << "Stalemate!" << endl;
        }
        return true;
    }
    if (wKingCheck) cout << "White is in check." << endl;
    if (bKingCheck) cout << "Black is in check." << endl;
    return false;
}

bool Board::pin(int row, int col, int destRow, int destCol) {

    // setting king's coord to appropriate one
    vector<int> kingCoords;
    if (grid[row][col].getPiece()->getColour() == 'W') kingCoords = wKingCoord;
    else kingCoords = bKingCoord;

    // piece and king diff in x direction
    int xDiff = col - kingCoords[1];
    
    // piece and dest. cell diff in x direction
    int xDestDiff = destCol - col;

    // piece and king diff in y direction
    int yDiff = row - kingCoords[0];

    // piece and dest. cell diff in y direction
    int yDestDiff = destRow - row;

    // checking if a piece in the line of attack can take king if the current piece moves out of the way
    // same col
    if (xDiff == 0) {
        // staying on the path
        if (xDestDiff == 0) return false;
        // piece in between king and cur
        if (grid[row][col].getPiece()->blocked(row, col, kingCoords[0], kingCoords[1], grid)) return false;
        if (yDiff < 0){
            for (int i = row-1; i >=0; --i){
                if (grid[i][col].getPiece()){
                    if (((grid[i][col].getPiece()->getType() == 'R') || (grid[i][col].getPiece()->getType() == 'Q'))  && 
                    (grid[i][col].getPiece()->getColour() != grid[row][col].getPiece()->getColour())){
                        return true;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        else {
            for (int i = row+1; i < 8; ++i){
                if (grid[i][col].getPiece()){
                    if (((grid[i][col].getPiece()->getType() == 'R') || (grid[i][col].getPiece()->getType() == 'Q'))  && 
                    (grid[i][col].getPiece()->getColour() != grid[row][col].getPiece()->getColour())){
                        return true;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        return false;
    }
    // same row
    else if (yDiff == 0) {
        // staying on the path
        if (yDestDiff == 0) return false;
        // piece in between king and cur
        if (grid[row][col].getPiece()->blocked(row, col, kingCoords[0], kingCoords[1], grid)) return false;
        if (xDiff < 0){
            for (int i = col-1; i >=0; --i){
                if (grid[row][i].getPiece()){
                    if (((grid[row][i].getPiece()->getType() == 'R') || (grid[row][i].getPiece()->getType() == 'Q'))  && 
                    (grid[row][i].getPiece()->getColour() != grid[row][col].getPiece()->getColour())){
                        return true;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        else {
            for (int i = col+1; i < 8; ++i){
                if (grid[row][i].getPiece()){
                    if (((grid[row][i].getPiece()->getType() == 'R') || (grid[row][i].getPiece()->getType() == 'Q'))  && 
                    (grid[row][i].getPiece()->getColour() != grid[row][col].getPiece()->getColour())){
                        return true;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        return false;
    }
    // diagonal
    else if (abs(xDiff) == abs(yDiff)) {
        // staying on the path
        if (yDiff*xDestDiff == yDestDiff*xDiff) return false;
        // piece in between king and cur
        if (grid[row][col].getPiece()->blocked(row, col, kingCoords[0], kingCoords[1], grid, true)) return false;
        if (xDiff == yDiff && xDiff > 0) {
            for (int i = 1;(col + i < 8) && (row + i < 8); ++i) {
                if (grid[row+i][col+i].getPiece()){
                    if(((grid[row+i][col+i].getPiece()->getType() == 'B') || 
                     (grid[row+i][col+i].getPiece()->getType() == 'Q')) && 
                     (grid[row+i][col+i].getPiece()->getColour() != grid[row][col].getPiece()->getColour())) {
                        return true;
                    }
                    else {
                        break;
                    }
                }
                   
            }
        }
        else if (xDiff == yDiff && xDiff < 0){
            for (int i = -1;(col + i >= 0) && (row + i >= 0); --i) {
                if (grid[row+i][col+i].getPiece()) {
                    if(((grid[row+i][col+i].getPiece()->getType() == 'B') || 
                     (grid[row+i][col+i].getPiece()->getType() == 'Q')) && 
                     (grid[row+i][col+i].getPiece()->getColour() != grid[row][col].getPiece()->getColour())) {
                        return true;
                    }
                    else {
                        break;
                    }
                }
                    
            }
        }
        else if (xDiff == -yDiff && xDiff < 0){
            for (int i = 1;(col - i >= 0) && (row + i < 8); ++i) {
                if (grid[row+i][col-i].getPiece()){
                    if (((grid[row+i][col-i].getPiece()->getType() == 'B') || 
                     (grid[row+i][col-i].getPiece()->getType() == 'Q')) && 
                     (grid[row+i][col-i].getPiece()->getColour() != grid[row][col].getPiece()->getColour())) {
                        return true;
                    }
                    else {
                        break;
                    }

                }
            }

        }
        else {
            for (int i = 1;(col + i < 8) && (row - i >= 0); ++i) {
                if (grid[row-i][col+i].getPiece()){
                    if (((grid[row-i][col+i].getPiece()->getType() == 'B') || 
                     (grid[row-i][col+i].getPiece()->getType() == 'Q')) && 
                     (grid[row-i][col+i].getPiece()->getColour() != grid[row][col].getPiece()->getColour())) {
                        return true;
                    }
                    else {
                        break;
                    }

                }
            }
        }
        return false;
    }
    return false;          
} 

void Board::changeTurn() {
    if (turn == 'W') {
        turn = 'B';
    } else {
        turn = 'W';
    }
}

void Board::move(string start, string dest, char promote) {
    // resetting enpas coords
    if ((enpasCoord[0] != 0) && (enpasCoord[0] == 2 && turn == 'B' || enpasCoord[0] == 5 && turn == 'W')) {
        enpasCoord[0] = 0;
        enpasCoord[1] = 0;
        grid[enpasCoord[0]][enpasCoord[1]].setEnpas(false);
    }

    // game loop
    while (true) {
     //   cout << start << dest << endl;
        int row = '8' - start[1];
        int col = start[0] - 'a';
        int rowDest = '8' - dest[1];
        int colDest = dest[0] - 'a';
   //     cout << row << ' ' << col << endl;
   //     cout << rowDest << ' ' << colDest << endl;
        bool invalid = false;

        vector<int> curKing;
        if (turn == 'W') curKing = wKingCoord;
        else curKing = bKingCoord;

        // edge cases
        // 1. empty cell
        if (!grid[row][col].getPiece()) {
            cout << "Invalid input! The given coordinates have no piece" << endl;
            invalid = true;
        // 2. check if user is trying to move opposite color
        } else if (grid[row][col].getPiece()->getColour() != turn) {
            cout << "Invalid input! The chosen piece does not belong to you" << endl;
            invalid = true;
        } 
        // 3. check for pinned
        else if (pin(row, col, rowDest, colDest)) {   // pinned
            cout << "Invalid input! The piece is pinned" << endl;
            invalid = true;
        }
        // 4. checking if move is valid
        else if (!grid[row][col].getPiece()->isValidMove(row, col, rowDest, colDest, grid, curKing, true)){
            cout << "Invalid input! The move is invalid" << endl;
            invalid = true;
        } 
        // 5. handling promotion
        else if ((grid[rowDest][colDest].getPiece()->getType() == 'P') && promote != 'P' && (rowDest == 0 || rowDest == 7)) {
            char colour = grid[rowDest][colDest].getPiece()->getColour();
            if (promote == 'Q') {
                grid[rowDest][colDest].setPiece(make_unique<Queen>(colour, rowDest, colDest));
            } else if (promote == 'R') {
                grid[rowDest][colDest].setPiece(make_unique<Rook>(colour, rowDest, colDest));
            } else if (promote == 'N') {
                grid[rowDest][colDest].setPiece(make_unique<Knight>(colour, rowDest, colDest));
            } else if (promote == 'B') {
                grid[rowDest][colDest].setPiece(make_unique<Bishop>(colour, rowDest, colDest));
            } 
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    grid[i][j].attach(grid[rowDest][colDest].getPiece());
                    grid[rowDest][colDest].getPiece()->attach(&(grid[i][j]));
                }
            }
        }
        
        // if invalid, don't change turn, get new inputs, and loop
        // fix infinite loop
        if (invalid) {
            cout << "Try again [move coord1 coord2]: ";
            string cmd;
            std::cin >> cmd;
            string line;
            string in;
            getline(std::cin, line);
            istringstream input {line};
            for (int i = 0; i < 3 && input >> in; ++i) {
                if (i == 0) {
                    start = in;
                } else if (i == 1) {
                    dest = in;
                } else {
                    promote = in[0];
                }
            }
        }
        // change turn if piece was moved
        else {
            // update pieceCoords
            // check if coords and grid have disagreements (due to move, castling, en passant)
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    // found in grid but not in pieceCoords
                    if (grid[i][j].getPiece()) {
                        if (grid[i][j].getPiece()->getColour() == 'W'){
                            bool found = false;
                            for (auto it = wPieceCoords.begin(); it != wPieceCoords.end(); ++it) {
                                if (*it == std::make_pair(i, j)) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) wPieceCoords.emplace_back(i, j);
                            // if a piece was captured
                            for (auto it = bPieceCoords.begin(); it != bPieceCoords.end();) {
                                if (*it == std::make_pair(i, j)) {
                                    it = bPieceCoords.erase(it);
                                    break;
                                } else {
                                    ++it;
                                }
                            }
                        }
                        else {
                            bool found = false;
                            for (auto it = bPieceCoords.begin(); it != bPieceCoords.end(); ++it) {
                                if (*it == std::make_pair(i, j)) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) bPieceCoords.emplace_back(i, j);
                            for (auto it = wPieceCoords.begin(); it != wPieceCoords.end();) {
                                if (*it == std::make_pair(i, j)) {
                                    it = wPieceCoords.erase(it);
                                    break;
                                } else {
                                    ++it;
                                }
                            }
                        }
                        // add it
                    } 
                    // found in pieceCoords but not in grid
                    else {
                        for (auto it = wPieceCoords.begin(); it != wPieceCoords.end();) {
                            if (*it == std::make_pair(i, j)) {
                                it = wPieceCoords.erase(it);
                                break;
                            } else {
                                ++it;
                            }
                        }
                        for (auto it = bPieceCoords.begin(); it != bPieceCoords.end();) {
                            if (*it == std::make_pair(i, j)) {
                                it = bPieceCoords.erase(it);
                                break;
                            } else {
                                ++it;
                            }
                        }
                        // remove it
                    }
                }
            }

            // resetting threats and captures
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (grid[i][j].getPiece()) {
                        grid[i][j].getPiece()->clearCaptures();
                    }
                    grid[i][j].clearThreats();
                }
            }

            // grid[wKingCoord[0]][wKingCoord[1]].notifyObservers(grid, wKingCoord, bKingCoord);
            // grid[bKingCoord[0]][bKingCoord[1]].notifyObservers(grid, wKingCoord, bKingCoord);

            // call notifyObservers of all cells to update threats (in cells) and captures (in pieces)
            // first, get correct threats
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    grid[i][j].notifyObservers(grid, wKingCoord, bKingCoord);
                }
            }

            // clear captures
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (grid[i][j].getPiece()) {
                        grid[i][j].getPiece()->clearCaptures();
                    }
                }
            }


            // get correct captures
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    grid[i][j].notifyObservers(grid, wKingCoord, bKingCoord);
                }
            }

            if (!grid[wKingCoord[0]][wKingCoord[1]].getBlackT().empty()) {
                wKingCheck = true;
            } else {
                wKingCheck = false;
            }
            if (!grid[bKingCoord[0]][bKingCoord[1]].getWhiteT().empty()) {
                bKingCheck = true;
            } else {
                bKingCheck = false;
            }
            
            changeTurn();
            break;
        }
    }
}

void Board::runGame(string player1, string player2) {
    // gameRunning = true;
    if (!setUpDone) {
        // placing the pawns
        for (int col = 0; col < 8; ++col) {
            grid[1][col].setPiece(make_unique<Pawn>('B', 1, col));
            grid[6][col].setPiece(make_unique<Pawn>('W', 6, col));
        }
        // black pieces
        grid[0][0].setPiece(make_unique<Rook>('B', 0, 0));
        grid[0][1].setPiece(make_unique<Knight>('B', 0, 1));
        grid[0][2].setPiece(make_unique<Bishop>('B', 0, 2));
        grid[0][3].setPiece(make_unique<Queen>('B', 0, 3));
        grid[0][4].setPiece(make_unique<King>('B', 0, 4));
        grid[0][5].setPiece(make_unique<Bishop>('B', 0, 5));
        grid[0][6].setPiece(make_unique<Knight>('B', 0, 6));
        grid[0][7].setPiece(make_unique<Rook>('B', 0, 7));

        // white pieces
        grid[7][0].setPiece(make_unique<Rook>('W', 7, 0));
        grid[7][1].setPiece(make_unique<Knight>('W', 7, 1));
        grid[7][2].setPiece(make_unique<Bishop>('W', 7, 2));
        grid[7][3].setPiece(make_unique<Queen>('W', 7, 3));
        grid[7][4].setPiece(make_unique<King>('W', 7, 4));
        grid[7][5].setPiece(make_unique<Bishop>('W', 7, 5));
        grid[7][6].setPiece(make_unique<Knight>('W', 7, 6));
        grid[7][7].setPiece(make_unique<Rook>('W', 7, 7));

        // Attach observers from the opposite color
        // adding coords of all pieces
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (grid[i][j].getPiece()) {
                    if (grid[i][j].getPiece()->getColour() == 'W') wPieceCoords.emplace_back(i, j);
                    else bPieceCoords.emplace_back(i, j);
                }
                attachObserverToCell(i, j, grid);
            }
        }
    }
    int p1 = 0;
    int p2 = 0;
    if (player1 != "human") p1 = player1[9] - '0';
    if (player2 != "human") p2 = player2[9] - '0';
    W = p1;
    B = p2;
    string cmd;
    cout << *td;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            grid[i][j].notifyObservers(grid, wKingCoord, bKingCoord);
        }
    }
    while (true) {
        // check for turn
        if (checkEnd()) break;
        if (turn == 'W') {
            cout << "White's turn!" << endl;
            if (W == 1) {
                botMove1();
            } else if (W == 2) {
                botMove2();
            } else if (W == 3) {
                botMove3();
            } else if (W == 4) {
                // botMove4();
            } else {
                std::cin >> cmd;
                if (cmd == "resign") {
                    cout << "Black wins!" << endl;
                    ++scoreB;
                    break;
                } else if (cmd == "move") {
                    bool invalid = false;
                    string line;
                    string in;
                    string start, dest;
                    char promote;
                    getline(std::cin, line);
                    istringstream input {line};
                    for (int i = 0; input >> in && i < 3; ++i) {
                        if (i == 0) {
                            start = in;
                            if (start[0] < 'a' || start[0] > 'h' || start[1] < '1' || start[1] > '8') {
                                cout << "invalid starting position" << endl;
                                invalid = true;
                                break;
                            }
                        } else if (i == 1) {
                            dest = in;
                            if (start[0] < 'a' || start[0] > 'h' || start[1] < '1' || start[1] > '8') {
                                cout << "invalid final position" << endl;
                                invalid = true;
                                break;
                            }
                        } else {
                            promote = in[0];
                            if (promote != 'Q' && promote != 'K' && promote != 'R' && promote != 'B' && promote != 'N' && promote != 'P') {
                                cout << "invalid piece type" << endl;
                                invalid = true;
                                break;
                            }
                        }
                    }
                    if (invalid) {
                        continue;
                    } else {
                        move(start, dest, promote);
                    }
                } else if (cin.eof()) {
                    break; 
                } else {
                    cout << "invalid command" << endl;
                    continue;
                }

            }
            cout << *td;
        } else {    
            cout << "Black's turn!" << endl;
            if (B == 1) {
                botMove1();
            } else if (B == 2) {
                botMove2();
            } else if (B == 3) {
                botMove3();
            } else if (B == 4) {
                // botMove4();
            }
            else {
                std::cin >> cmd;
                if (cmd == "resign") {
                    cout << "White wins!" << endl;
                    ++scoreW;
                    break;
                } else if (cmd == "move") {
                    bool invalid = false;
                    string line;
                    string in;
                    string start, dest;
                    char promote;
                    getline(std::cin, line);
                    istringstream input {line};
                    for (int i = 0; input >> in && i < 3; ++i) {
                        if (i == 0) {
                            start = in;
                            if (start[0] < 'a' || start[0] > 'h' || start[1] < '1' || start[1] > '8') {
                                cout << "invalid starting position" << endl;
                                invalid = true;
                                break;
                            }
                        } else if (i == 1) {
                            dest = in;
                            if (start[0] < 'a' || start[0] > 'h' || start[1] < '1' || start[1] > '8') {
                                cout << "invalid final position" << endl;
                                invalid = true;
                                break;
                            }
                        } else {
                            promote = in[0];
                            if (promote != 'Q' && promote != 'K' && promote != 'R' && promote != 'B' && promote != 'N' && promote != 'P') {
                                cout << "invalid piece type" << endl;
                                invalid = true;
                                break;
                            }
                        }
                    }
                    if (invalid) {
                        continue;
                    } else {
                        move(start, dest, promote);
                        
                    }
                } else if (cin.eof()) {
                    break;
                } else {
                    cout << "invalid command" << endl;
                    continue;
                }
            }
            cout << *td;
        }
    }
}

void Board::reset() {
    // resetting board
    setUpDone = false;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            grid[row][col].setPiece(nullptr);
        }
    }

    wPieceCoords.clear();
    bPieceCoords.clear();
    bKingCoord = {0,4};
    wKingCoord = {7,4};
    enpasCoord = {0,0};
    turn = 'W';
    wKingCheck = false;
    bKingCheck = false;
    W = 0;
    B = 0;
}

void Board::setUp() {
    int wKCount = 0;        // number of white king on the board
    int bKCount = 0;        // number of black king on the board
    bool wInCheck = false;
    bool bInCheck = false;
    // if (gameRunning) {
    //     cout << "Game is running. Cannot enter setup mode." << endl;
    // } else {
        string cmd;
        while (true) {
            std::cin >> cmd;
            if (cin.eof()) {
                break;
            } else if (cmd == "+") {
                bool invalid = false;
                char piece;
                string start;
                char colour;
                int fileNum;
                int rankNum;
                getline(std::cin, cmd);
                istringstream line {cmd};
                for (int i = 0; i < 2; ++i) {
                    if (i == 0) {
                        if (!(line >> piece) ||
                         (piece != 'Q' && piece != 'q' && piece != 'K' && piece != 'k' && piece != 'R' && piece != 'r' && piece != 'B' && piece != 'b' && piece != 'N' && piece != 'n' && piece != 'P' && piece != 'p')) {
                            cout << "invalid piece" << endl;
                            invalid = true;
                        }
                    }
                    if (i == 1) {
                        if ((line >> start)) {
                            fileNum = start[0] - 'a';
                            rankNum = '8' - start[1];
                            if (fileNum < 0 || fileNum > 7) {
                                cout << "invalid file" << endl;
                                invalid = true;
                            }
                            if (rankNum < 0 || rankNum > 7) {
                                cout << "invalid rank" << endl;
                                invalid = true;
                            }
                        } else {
                            cout << "invalid input" << endl;
                            invalid = true;
                        }
                    }
                }
                if (invalid) {
                    cout << "Try again" << endl;
                    invalid = false;
                    continue;
                }
                if (piece >= 'A' && piece <= 'Z') {
                    colour = 'W';
                } else {
                    colour = 'B';
                }
                if (piece == 'R' || piece == 'r') {
                    grid[rankNum][fileNum].setPiece(make_unique<Rook>(colour, rankNum, fileNum));
                } else if (piece == 'B' || piece == 'b') {
                    grid[rankNum][fileNum].setPiece(make_unique<Bishop>(colour, rankNum, fileNum));
                } else if (piece == 'N' || piece == 'n') {
                    grid[rankNum][fileNum].setPiece(make_unique<Knight>(colour, rankNum, fileNum));
                } else if (piece == 'Q' || piece == 'q') {
                    // cout << fileNum << rank << piece << endl;
                    grid[rankNum][fileNum].setPiece(make_unique<Queen>(colour, rankNum, fileNum));
                } else if (piece == 'K' || piece == 'k') {
                    grid[rankNum][fileNum].setPiece(make_unique<King>(colour, rankNum, fileNum));
                    if (colour == 'W') {
                        ++wKCount;
                    } else {
                        ++bKCount;
                    }
                } else {
                    grid[rankNum][fileNum].setPiece(make_unique<Pawn>(colour, rankNum, fileNum));
                }
            } else if (cmd == "-") {
                bool invalid = false;
                string coord;
                char file;
                char rank;
                int fileNum;
                int rankNum;
                for (int i = 0; i < 2; ++i) {
                    if (i == 0) {
                        if (cin >> file) {
                            fileNum = file - 'a';
                            if (fileNum < 0 || fileNum > 7) {
                                cout << "invalid file" << endl;
                                invalid = true;
                            }
                        } else {
                            cout << "invalid output" << endl;
                        }
                    }
                    if (i == 1) {
                        if (cin >> rank) {
                            rankNum = '8' - rank;
                            if (rankNum < 0 || rankNum > 7) {
                                cout << "invalid rank" << endl;
                                invalid = true;
                            }
                        } else {
                            cout << "invalid output" << endl;
                        }
                    } 
                }
                if (invalid) {
                    cout << "Try again" << endl;
                    invalid = true;
                    continue;
                }
                Piece *p = grid[rankNum][fileNum].getPiece();
                if (p) {
                    if (p->getType() == 'K') {
                        if (p->getColour() == 'W') {
                            --wKCount;
                        } else {
                            --bKCount;
                        }
                    }
                    grid[rankNum][fileNum].setPiece(nullptr);
                }
                for (auto it = wPieceCoords.begin(); it != wPieceCoords.end();) {
                    if (*it == std::make_pair(rankNum, fileNum)) {
                        it = wPieceCoords.erase(it);
                        break;
                    } else {
                        ++it;
                    }
                }
                for (auto it = bPieceCoords.begin(); it != bPieceCoords.end();) {
                    if (*it == std::make_pair(rankNum, fileNum)) {
                        it = bPieceCoords.erase(it);
                        break;
                    } else {
                        ++it;
                    }
                }
            } else if (cmd == "=") {
                char colour;
                if (!(cin >> colour) || (colour != 'W' && colour != 'B')) {
                    cout << "invalid colour" << endl;
                    cout << "Try again" << endl;
                    continue;
                }
                turn = colour;
                cout << turn << endl;
                if (colour == 'W') {
                    cout << "Set turn to White" << endl;
                } else {
                    cout << "Set turn to Black" << endl;
                }
            } else if (cmd == "done") {
                bool invalid = false;
                // cout << wKCount << bKCount << endl;
                if (wKCount != 1 || bKCount != 1) {
                    cout << "There should be exactly one white king and exactly one black king" << endl;
                    invalid = true;
                }
                bool wKingFound = false;
                bool bKingFound = false;
                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        if (grid[i][j].getPiece()) {
                            if (grid[i][j].getPiece()->getType() == 'K') {
                                if (grid[i][j].getPiece()->getColour() == 'W') {
                                    wKingCoord[0] = i;
                                    wKingCoord[1] = j;
                                    wKingFound = true;
                                } else {
                                    bKingCoord[0] = i;
                                    bKingCoord[1] = j; 
                                    bKingFound = true;
                                }
                            }
                        }
                        if (wKingFound && bKingFound) {
                            break;
                        }
                    }
                }
                for (int c = 0; c < 8; ++c) {
                    Piece *top = grid[0][c].getPiece();
                    Piece *bottom = grid[7][c].getPiece();
                    if (top && top->getType() == 'P') {
                        cout << "There should be no pawn on the first row of the board" << endl;
                        invalid = true;
                    }
                    if (bottom && bottom->getType() == 'P') {
                        cout << "There should be no pawn on the last row of the board" << endl;
                        invalid = true;
                    }
                    if (invalid) break;
                }
                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        if (grid[i][j].getPiece()) {
                            if (grid[i][j].getPiece()->getColour() == 'W') wPieceCoords.emplace_back(i, j);
                            else bPieceCoords.emplace_back(i, j);
                        }
                        attachObserverToCell(i, j, grid);
                    }
                }
                // resetting threats and captures
                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        if (grid[i][j].getPiece()) {
                            grid[i][j].getPiece()->clearCaptures();
                        }
                        grid[i][j].clearThreats();
                    }
                }
                // call notifyObservers of all cells to update threats (in cells) and captures (in pieces)
                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        grid[i][j].notifyObservers(grid, wKingCoord, bKingCoord);
                    }
                }
                inCheck('W');
                inCheck('B');
                if (wKingCheck || bKingCheck) {
                    cout << "Neither king should be in check" << endl;
                    invalid = true;
                }
                if (invalid) {
                    cout << "Try again" << endl;
                    continue;
                } else {
                    setUpDone = true;
                    break;
                }
            } else {
                cout << "invalid command" << endl;
                continue;
            }
    }
}


void Board::notify(Cell &c) {
    if (c.getPiece()) {
        char type = c.getPiece()->getType();
        char colour = c.getPiece()->getColour();
        if (type == 'K') {
            if (colour == 'W') {
                wKingCoord[0] = c.r;
                wKingCoord[1] = c.c;
            } else {
                bKingCoord[0] = c.r;
                bKingCoord[1] = c.c;
            }
        }
    }
    if (c.getEnpas()) {
        enpasCoord[0] = c.getRow();
        enpasCoord[1] = c.getCol();
    }
}

vector<pair<int,int>> Board::coordsOfMove() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng{seed};
    vector<pair<int,int>> randomMove;
    if (turn == 'W'){
        vector<pair<int,int>> wCoords = wPieceCoords; // don't want to directly change the coordinates
        shuffle(wCoords.begin(), wCoords.end(), rng);
        for (auto i: wCoords){
            if (grid[i.first][i.second].getPiece()->getCaptures().size() == 0){
                continue;
            }
            else {
                vector<pair<int,int>> coords = grid[i.first][i.second].getPiece()->getCaptures();
                shuffle(coords.begin(), coords.end(), rng);
                for (auto j: coords){
                    if(grid[i.first][i.second].getPiece()->isValidMove(i.first, i.second, j.first, j.second, grid, wKingCoord, false) && 
                    !pin(i.first, i.second, j.first, j.second)){
                        randomMove.emplace_back(i.first, i.second);
                        randomMove.emplace_back(j.first, j.second);
                        return randomMove;
                    }
                }
            }
        }
    }
    else {
        vector<pair<int,int>> bCoords = bPieceCoords;
        shuffle(bCoords.begin(), bCoords.end(), rng);
        for (auto i: bCoords){
            if (grid[i.first][i.second].getPiece()->getCaptures().size() == 0){
                continue;
            }
            else {
                vector<pair<int,int>> coords = grid[i.first][i.second].getPiece()->getCaptures();
                shuffle(coords.begin(), coords.end(), rng);
                for (auto j: coords){
                    if(grid[i.first][i.second].getPiece()->isValidMove(i.first, i.second, j.first, j.second, grid, bKingCoord, false) &&
                    !pin(i.first, i.second, j.first, j.second)){
                        randomMove.emplace_back(i.first, i.second);
                        randomMove.emplace_back(j.first, j.second);
                        return randomMove;
                    }
                }
            }
        }
    }
}


void Board::botMove1() {
    vector<pair<int,int>> movingPiece;
//     int size;
//     if (turn == 'W'){
//         size = wPieceCoords.size();
//     }
//     else {
//         size = bPieceCoords.size();
//     }
//  //   while(true) {
    movingPiece = coordsOfMove();
//  //       if (!pin(movingPiece[0].first, movingPiece[0].second, movingPiece[1].first, movingPiece[1].second)) break;
//  //   }
    if (movingPiece.size() == 0) {
        if (turn == 'W') {
            wPieceCoords.clear();
        }
        else {
            bPieceCoords.clear();
        }
        moveable();
    }
    else {
        char startRow = '8' - movingPiece[0].first;
        char startCol = 'a' + movingPiece[0].second;
        char endRow = '8' - movingPiece[1].first;
        char endCol = 'a' + movingPiece[1].second;
        string start = "";
        start += startCol;
        start += startRow;
        string dest = "";
        dest += endCol;
        dest += endRow;
        if (grid[movingPiece[0].first][movingPiece[0].second].getPiece()->getType() == 'P' && ((movingPiece[1].first == 0) || (movingPiece[1].first == 7) )){
            move(start, dest, 'Q');
        } else {
            move(start, dest);
        }
    }
}


void Board::botMove2() {
    vector<pair<int,int>> potentialMove;
    if (turn == 'W') {
        vector<pair<int,int>> wCoords = wPieceCoords; // don't want to directly change the coordinates
        for (auto i: wCoords){
            if (grid[i.first][i.second].getPiece()){
                            if (grid[i.first][i.second].getPiece()->getCaptures().size() == 0){
                continue;
            }
            else {
                vector<pair<int,int>> coords = grid[i.first][i.second].getPiece()->getCaptures();
                for (auto j: coords){
                    if (grid[j.first][j.second].getPiece()){
                        if(grid[i.first][i.second].getPiece()->getColour() == turn && grid[i.first][i.second].getPiece()->isValidMove(i.first, i.second, j.first, j.second, grid, bKingCoord, false) &&
                    !pin(i.first, i.second, j.first, j.second)){
                        potentialMove.emplace_back(i.first, i.second);
                        potentialMove.emplace_back(j.first, j.second);
                        }
                    }
                }
            }

            }
        }
        if (potentialMove.empty() || grid[potentialMove[0].first][potentialMove[0].second].isEmpty()  || grid[potentialMove[0].first][potentialMove[0].second].getPiece()->getColour() != turn){
                botMove1();
            } else{
                char startRow = '8' - potentialMove[0].first;
                char startCol = 'a' + potentialMove[0].second;
                char endRow = '8' - potentialMove[1].first;
                char endCol = 'a' + potentialMove[1].second;
                string start = "";
                start += startCol;
                start += startRow;
                string dest = "";
                dest += endCol;
                dest += endRow;
                if (grid[potentialMove[0].first][potentialMove[0].second].getPiece()->getType() == 'P' && ((potentialMove[1].first == 0) || (potentialMove[1].first == 7))){
                    move(start, dest, 'Q');
                } else {
                    move(start, dest);
                }

            }
    }
    else {
        vector<pair<int,int>> bCoords = bPieceCoords; // don't want to directly change the coordinates
        for (auto i: bCoords){
            if (grid[i.first][i.second].getPiece()){
                if (grid[i.first][i.second].getPiece()->getCaptures().size() == 0){
                    continue;
                }
            else {
                vector<pair<int,int>> coords = grid[i.first][i.second].getPiece()->getCaptures();
                for (auto j: coords){
                    if (grid[j.first][j.second].getPiece()){
                        if(grid[i.first][i.second].getPiece()->getColour() == turn && grid[i.first][i.second].getPiece()->isValidMove(i.first, i.second, j.first, j.second, grid, bKingCoord, false) &&
                    !pin(i.first, i.second, j.first, j.second)){
                        potentialMove.emplace_back(i.first, i.second);
                        potentialMove.emplace_back(j.first, j.second);
                        }
                    }
                }
            }
        }
            
        }
        if (potentialMove.empty() || grid[potentialMove[0].first][potentialMove[0].second].isEmpty() || grid[potentialMove[0].first][potentialMove[0].second].getPiece()->getColour() != turn){
                botMove1();
            } else{
                char startRow = '8' - potentialMove[0].first;
                char startCol = 'a' + potentialMove[0].second;
                char endRow = '8' - potentialMove[1].first;
                char endCol = 'a' + potentialMove[1].second;
                string start = "";
                start += startCol;
                start += startRow;
                string dest = "";
                dest += endCol;
                dest += endRow;
                if (grid[potentialMove[0].first][potentialMove[0].second].getPiece()->getType() == 'P' && ((potentialMove[1].first == 0) || (potentialMove[1].first == 7))){
                    move(start, dest, 'Q');
                } else {
                    move(start, dest);
                }

            }
    }
}

void Board::botMove3() {
    vector<pair<int,int>> potentialMove;
    if (turn == 'W') {
        vector<pair<int,int>> wCoords = wPieceCoords; // don't want to directly change the coordinates
        for (auto i: wCoords){
            if (grid[i.first][i.second].getPiece()){
                if (grid[i.first][i.second].getPiece()->getCaptures().size() == 0 || grid[i.first][i.second].getBlackT().size() == 0){
                continue;
            }
            else {
                vector<pair<int,int>> coords = grid[i.first][i.second].getPiece()->getCaptures();
                for (auto j: coords){
                        if(grid[i.first][i.second].getPiece()->getColour() == turn && grid[i.first][i.second].getPiece()->isValidMove(i.first, i.second, j.first, j.second, grid, bKingCoord, false) &&
                    !pin(i.first, i.second, j.first, j.second)){
                        potentialMove.emplace_back(i.first, i.second);
                        potentialMove.emplace_back(j.first, j.second);
                    }
                }
            }

            }
        }
        if (potentialMove.empty() || grid[potentialMove[0].first][potentialMove[0].second].isEmpty()){
                botMove2();
            } else{
                char startRow = '8' - potentialMove[0].first;
                char startCol = 'a' + potentialMove[0].second;
                char endRow = '8' - potentialMove[1].first;
                char endCol = 'a' + potentialMove[1].second;
                string start = "";
                start += startCol;
                start += startRow;
                string dest = "";
                dest += endCol;
                dest += endRow;
                if (grid[potentialMove[0].first][potentialMove[0].second].getPiece()->getType() == 'P' && ((potentialMove[1].first == 0) || (potentialMove[1].first == 7))){
                    move(start, dest, 'Q');
                } else {
                    move(start, dest);
                }

            }
    }
    else {
        vector<pair<int,int>> bCoords = bPieceCoords; // don't want to directly change the coordinates
        for (auto i: bCoords){
            if (grid[i.first][i.second].getPiece()){
                if (grid[i.first][i.second].getPiece()->getCaptures().size() == 0 || grid[i.first][i.second].getWhiteT().size() == 0){
                    continue;
                }
            else {
                vector<pair<int,int>> coords = grid[i.first][i.second].getPiece()->getCaptures();
                for (auto j: coords){
                        if(grid[i.first][i.second].getPiece()->getColour() == turn && grid[i.first][i.second].getPiece()->isValidMove(i.first, i.second, j.first, j.second, grid, bKingCoord, false) &&
                    !pin(i.first, i.second, j.first, j.second)){
                        potentialMove.emplace_back(i.first, i.second);
                        potentialMove.emplace_back(j.first, j.second);
                        }
                }
            }
        }
        }
        if (potentialMove.empty() || grid[potentialMove[0].first][potentialMove[0].second].isEmpty()){
                botMove2();
            } else{
                char startRow = '8' - potentialMove[0].first;
                char startCol = 'a' + potentialMove[0].second;
                char endRow = '8' - potentialMove[1].first;
                char endCol = 'a' + potentialMove[1].second;
                string start = "";
                start += startCol;
                start += startRow;
                string dest = "";
                dest += endCol;
                dest += endRow;
                if (grid[potentialMove[0].first][potentialMove[0].second].getPiece()->getType() == 'P' && ((potentialMove[1].first == 0) || (potentialMove[1].first == 7))){
                    move(start, dest, 'Q');
                } else {
                    move(start, dest);
                }

            }
    }

}
// void botMove4() {}

int Board::getWScore() {return scoreW;}
int Board::getBScore() {return scoreB;}
