#include <iostream>
#include <string>
#include "board.h"
#include "window.h"
using namespace std;

int main() {
    string cmd;
    Xwindow xw;
    Board b {xw};
    while (true) {
        cout << "Enter setup/game: " << endl;
        if (cin >> cmd) {
            b.reset();
            if (cmd == "game") {
                string player1, player2;
                cin >> player1 >> player2;
                b.runGame(player1, player2);
            } else if (cmd == "setup") {
                b.setUp();
            } else {
                cout << "invalid command" << endl;
            }
        } else {
            break;
        }
    }
    cout << "Final score:" << endl;
    cout << "White: " << b.getWScore() << endl;
    cout << "Black: " << b.getBScore() << endl;
    return 0;
}