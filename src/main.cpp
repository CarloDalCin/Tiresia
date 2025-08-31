// STD
#include <iostream>
#include <string>

// Tiresia
#include "libtiresia.hpp"

int main() {
    std::string line;

    // board of the game witch will be used to test the capabilites of the library with CuteChess
    board_t board;
    initBoardSTD(&board);
    printBoard(&board);

    while (std::getline(std::cin, line)) {
        if (line == "uci") {
            std::cout << "id name Tiresia 1.0\n";
            std::cout << "id author github.com/CarloDalCin\n";
            std::cout << "uciok" << std::endl;
        }
        else if (line == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (line == "quit") {
            break;
        }
        else if (line.rfind("position", 0) == 0) {
            // Qui dovresti caricare la posizione in base alla FEN o alle mosse
        }
        else if (line.rfind("go", 0) == 0) {
            std::cout << "bestmove " /*<< bestMove() */ << std::endl;
        }
    }

    return 0;
}
