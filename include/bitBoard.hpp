#pragma once

#include <cstdint>

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

// White pieces bitboards
uint64_t whiteKing   = cell_to_ULL(5,1);
uint64_t whiteQueen  = cell_to_ULL(4,1);
uint64_t whiteRook   = cell_to_ULL(1,1) | cell_to_ULL(8,1);
uint64_t whiteKnight = cell_to_ULL(2,1) | cell_to_ULL(7,1);
uint64_t whiteBishop = cell_to_ULL(3,1) | cell_to_ULL(6,1);
uint64_t whitePawn   = 0xFFULL << 8;

// Black pieces bitboards
uint64_t blackKing   = cell_to_ULL(5,8);
uint64_t blackQueen  = cell_to_ULL(4,8);
uint64_t blackRook   = cell_to_ULL(1,8) | cell_to_ULL(8,8);
uint64_t blackKnight = cell_to_ULL(2,8) | cell_to_ULL(7,8);
uint64_t blackBishop = cell_to_ULL(3,8) | cell_to_ULL(6,8);
uint64_t blackPawn   = 0xFFULL << 48;

uint64_t whitePieces = whiteKing | whiteQueen | whiteRook | whiteKnight | whiteBishop | whitePawn;
uint64_t blackPieces = blackKing | blackQueen | blackRook | blackKnight | blackBishop | blackPawn;

uint64_t allPieces = whitePieces | blackPieces;

void printBoard() {
    std::cout << "  a b c d e f g h\n";
    for (int rank = 8; rank >= 1; --rank) {
        std::cout << rank << " ";
        for (int file = 1; file <= 8; ++file) {
            uint64_t mask = cell_to_ULL(file, rank);
            std::string piece;

            if (whiteKing & mask) piece = "♚";
            else if (whiteQueen & mask) piece = "♛";
            else if (whiteRook & mask) piece = "♜";
            else if (whiteKnight & mask) piece = "♞";
            else if (whiteBishop & mask) piece = "♝";
            else if (whitePawn & mask) piece = "♟";
            else if (blackKing & mask) piece = "♔";
            else if (blackQueen & mask) piece = "♕";
            else if (blackRook & mask) piece = "♖";
            else if (blackKnight & mask) piece = "♘";
            else if (blackBishop & mask) piece = "♗";
            else if (blackPawn & mask) piece = "♙";
            else piece = ".";

            std::cout << piece << " ";
        }
        std::cout << rank << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}
