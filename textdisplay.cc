#include "textdisplay.h"

TextDisplay::TextDisplay() {
    for (int r = 0; r < 8; ++r) {
        std::vector<char> row;      // construct the row
        for (int c = 0; c < 8; ++c) {
            // if (r == 0) {       // when r == 0 (rank == 8)
            //     if (c == 0 || c == 7) {     // when c == 0 or c == 7 (file == 1 or file == 8), add a black rook
            //         row.emplace_back('r');
            //     } else if (c == 1 || c == 6) {      // when c == 1 or c == 6 (file == 2 or file == 7), add a black knight
            //         row.emplace_back('n');
            //     } else if (c == 2 || c == 5) {      // when c == 2 or c == 5 (file == 3 or file == 6), add a black bishop
            //         row.emplace_back('b');
            //     } else if (c == 3) {        // when c == 3 (file == 4), add a black queen
            //         row.emplace_back('q');
            //     } else {        // when c == 4 (file == 5), add a black king
            //         row.emplace_back('k');
            //     }
            // } else if (r == 1) {       // when r == 1 (rank == 7), add black pawns
            //     row.emplace_back('p');
            // } else if (r == 6) {       // when r == 6 (rank == 2), add white pawns
            //     row.emplace_back('P');
            // } else if (r == 7) {       // when r == 7 (rank == 1)
            //     if (c == 0 || c == 7) {     // when c == 0 or c == 7 (file == 1 or file == 8), add a white rook
            //         row.emplace_back('R');
            //     } else if (c == 1 || c == 6) {      // when c == 1 or c == 6 (file == 2 or file == 7), add a white knight
            //         row.emplace_back('N');
            //     } else if (c == 2 || c == 5) {      // when c == 2 or c == 5 (file == 3 or file == 6), add a white bishop
            //         row.emplace_back('B');
            //     } else if (c == 3) {             // when c == 3 (file == 4), add a white queen
            //         row.emplace_back('Q');
            //     } else {         // when c == 4 (file == 5), add a white king
            //         row.emplace_back('K');
            //     }
            // } else {
            if ((r + c) % 2 == 0) {
                row.emplace_back(' ');
            } else {
                row.emplace_back('_');
            }
        }
        theDisplay.emplace_back(row);
    }
}

void TextDisplay::notify(Cell &c) {
    // cout << "td ";
    int row = c.getRow();
    int col = c.getCol();
    Piece *p = c.getPiece();
    if (p != nullptr) {          // if the piece is not an Empty
        char type = p->getType();
        char colour = p->getColour();
        if (colour == 'B') {
            type += 32;     // change the type to lower case for output
        }
        theDisplay[row][col] = type;
    } else {        // if the piece is an Empty
        if ((row + col) % 2 == 0) {
            theDisplay[row][col] = ' ';
        } else {
            theDisplay[row][col] = '_';
        }
    }
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    for (int r = 0; r < 8; ++r) {
        int rank = 8 - r;
        out << rank << " ";
        for (int c = 0; c < 8; ++c) {
            out << td.theDisplay[r][c];
        }
        out << endl;
    }
    out << endl;
    out << "  abcdefgh" << endl;
    return out;
}
