#include "graphicsdisplay.h"

GraphicsDisplay::GraphicsDisplay(Xwindow &xw): xw{xw} {
    // draws the bottom and left sides for indicating files and ranks
    xw.fillRectangle(0, 0, 500, 20, 0);
    xw.fillRectangle(0, 0, 20, 500, 0);
    for (int r = 0; r < 8; ++r) {
        ostringstream rank;
        char rankC = '8' - r;
        rank << rankC;
        xw.drawString(10, 50 + r * 60, rank.str());       // draws a string indicating rank on each r (row)
        for (int c = 0; c < 8; ++c) {
            ostringstream file;
            char fileC = 'a' + c;
            file << fileC;
            xw.drawString(50 + c * 60, 10, file.str());
            if ((r + c) % 2 == 0) {         // create the "squares" with green and blue colours alternating
                xw.fillRectangle(20 + c * 60, 20 + r * 60, 60, 60, 3);      
            } else {
                xw.fillRectangle(20 + c * 60, 20 + r * 60, 60, 60, 4); 
            }
            if (r == 0) {       // when r == 0 (rank == 8)
                ostringstream file;
                char fileC = 'a' + c;
                file << fileC;
                xw.drawString(50 + c * 60, 10, file.str());       // draws a string indicating file on each c (column)
            }
            //     if (c == 0 || c == 7) {     // when c == 0 or c == 7 (file == 1 or file == 8), draws a black rook
            //         xw.drawPiece(30 + c * 60, 30, 'R', 1, 40);
            //     } else if (c == 1 || c == 6) {      // when c == 1 or c == 6 (file == 2 or file == 7), draws a black knight
            //         xw.drawPiece(30 + c * 60, 30, 'N', 1, 40);
            //     } else if (c == 2 || c == 5) {      // when c == 2 or c == 5 (file == 3 or file == 6), draws a black bishop
            //         xw.drawPiece(30 + c * 60, 30, 'B', 1, 40);
            //     } else if (c == 3) {        // when c == 3 (file == 4), draws a black queen
            //         xw.drawPiece(30 + c * 60, 30, 'Q', 1, 40);
            //     } else {        // when c == 4 (file == 5), draws a black king
            //         xw.drawPiece(30 + c * 60, 30, 'K', 1, 40);
            //     }
            // }
            // if (r == 1) {       // when r == 1 (rank == 7), draws black pawns
            //     xw.drawPiece(30 + c * 60, 30 + r * 60, 'P', 1, 40); 
            // }
            // if (r == 6) {       // when r == 6 (rank == 2), draws white pawns
            //     xw.drawPiece(30 + c * 60, 30 + r * 60, 'P', 0, 40);
            // }
            // if (r == 7) {       // when r == 7 (rank == 1)
            //     if (c == 0 || c == 7) {     // when c == 0 or c == 7 (file == 1 or file == 8), draws a white rook
            //         xw.drawPiece(30 + c * 60, 30 + r * 60, 'R', 0, 40);
            //     } else if (c == 1 || c == 6) {      // when c == 1 or c == 6 (file == 2 or file == 7), draws a white knight
            //         xw.drawPiece(30 + c * 60, 30 + r * 60, 'N', 0, 40);
            //     } else if (c == 2 || c == 5) {      // when c == 2 or c == 5 (file == 3 or file == 6), draws a white bishop
            //         xw.drawPiece(30 + c * 60, 30 + r * 60, 'B', 0, 40);
            //     } else if (c == 3) {             // when c == 3 (file == 4), draws a white queen
            //         xw.drawPiece(30 + c * 60, 30 + r * 60, 'Q', 0, 40);
            //     } else {         // when c == 4 (file == 5), draws a white king
            //         xw.drawPiece(30 + c * 60, 30 + r * 60, 'K', 0, 40);
            //     }
            // }
        }
    }
}

void GraphicsDisplay::notify(Cell &c) {
    // cout << "gd" << endl;
    int row = c.getRow();
    int col = c.getCol();
    Piece *p = c.getPiece();
    // if the cell has a piece, draw the piece on the cell
    if (p != nullptr) {
        char type = p->getType();
        char colour = p->getColour();
        int colourNum = 0;
        if (colour == 'B') {
            colourNum = 1;
        }
        xw.drawPiece(30 + col * 60, 30 + row * 60, type, colourNum, 40);
    } else {        // if the cell has no piece, colour the cell with correct colour
    
       if ((row + col) % 2 == 0) {         // create the "squares" with green and blue colours alternating
                
                xw.fillRectangle(20 + col * 60, 20 + row * 60, 60, 60, 3); 
                // cout << "done graphic" << endl;     
            } else {
                xw.fillRectangle(20 + col * 60, 20 + row * 60, 60, 60, 4); 
            }
    }
}
