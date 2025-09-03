#pragma once

#include <cstdint>

#include "types.hpp"

/* 
  The board is represented as a 64 bit integer, with each bit representing a cells

  Board representation
| Casella | Bit index |
| ------- | --------- |
| a1      | 0         |
| b1      | 1         |
| ...     | ...       |
| h1      | 7         |
| a2      | 8         |
| ...     | ...       |
| h8      | 63        |
*/

#define cell_to_ULL(x, y) (1ULL << ((x-1) + (y-1)*8))

typedef struct {
    union {
        struct {
            // index 0 = all pieces of the same color indexable with 'all'
            uint64_t white[PIECE_NB];
            uint64_t black[PIECE_NB];
        };
        uint64_t pieces[2][PIECE_NB]; // 0 = white, 1 = black
    };
    uint64_t unionBitboard;
} board_t;

void initBoardSTD(board_t* b) {
    b->white[pawn]   = 0xFFULL << 8;
    b->white[knight] = cell_to_ULL(2,1) | cell_to_ULL(7,1);
    b->white[bishop] = cell_to_ULL(3,1) | cell_to_ULL(6,1);
    b->white[rook]   = cell_to_ULL(1,1) | cell_to_ULL(8,1);
    b->white[queen]  = cell_to_ULL(4,1);
    b->white[king]   = cell_to_ULL(5,1);
    b->white[all] = b->white[king] | b->white[queen] | b->white[rook] | b->white[knight] | b->white[bishop] | b->white[pawn];

    b->black[pawn]   = 0xFFULL << 48;
    b->black[knight] = cell_to_ULL(2,8) | cell_to_ULL(7,8);
    b->black[bishop] = cell_to_ULL(3,8) | cell_to_ULL(6,8);
    b->black[rook]   = cell_to_ULL(1,8) | cell_to_ULL(8,8);
    b->black[queen]  = cell_to_ULL(4,8);
    b->black[king]   = cell_to_ULL(5,8);
    b->black[all] = b->black[king] | b->black[queen] | b->black[rook] | b->black[knight] | b->black[bishop] | b->black[pawn];
    b->unionBitboard = b->white[0] | b->black[0];
}

// TODO
// void initBoard960(board_t* b);

void printBoard(board_t* b) {
    std::cout << "  a b c d e f g h\n";
    for (int rank = 8; rank >= 1; --rank) {
        std::cout << rank << " ";
        for (int file = 1; file <= 8; ++file) {
            uint64_t mask = cell_to_ULL(file, rank);
            std::string piece;

            if (b->white[king] & mask) piece = "♚";
            else if (b->white[queen] & mask) piece = "♛";
            else if (b->white[rook] & mask) piece = "♜";
            else if (b->white[knight] & mask) piece = "♞";
            else if (b->white[bishop] & mask) piece = "♝";
            else if (b->white[pawn] & mask) piece = "♟";
            else if (b->black[king] & mask) piece = "♔";
            else if (b->black[queen] & mask) piece = "♕";
            else if (b->black[rook] & mask) piece = "♖";
            else if (b->black[knight] & mask) piece = "♘";
            else if (b->black[bishop] & mask) piece = "♗";
            else if (b->black[pawn] & mask) piece = "♙";
            else piece = ".";

            std::cout << piece << " ";
        }
        std::cout << rank << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}
