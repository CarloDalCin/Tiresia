#pragma once

#include <cstdint>

#include "castle.hpp"
#include "move.hpp"
#include "piece.hpp"

/*
The board is represented as a 64 bit integer, with each bit representing a
cells

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

for a better understanding of how the board is represented see the move.hpp file
*/

class Board {
private:
  // total size = 1472 bits = 184 bytes
  Piece board[64];                // 64 * 8 bits = 512 bits
  CastleRights castleRights;      // 8 bits + 56 bits(padding) = 64 bits
  union {                         // 2 * 7 * 64 bits = 896 bits
    uint64_t pieces[2][PIECE_NB]; // 0 = white, 1 = black
    struct {
      // index 0 = all pieces of the same color the union of all white or black
      // pieces
      uint64_t white[PIECE_NB];
      uint64_t black[PIECE_NB];
    };
  };

private:
public:
  Board() = default;
  constexpr Board(const Board &b) = default;
  Board &operator=(const Board &b) = default;

  constexpr void print() const;

public:
  static Board init_std();
  static Board init_960();
};

// TODO
// void initBoard960(board_t* b);

// void printBoard(Board &b) {
//   std::cout << "  a b c d e f g h\n";
//   for (int rank = 8; rank >= 1; --rank) {
//     std::cout << rank << " ";
//     for (int file = 1; file <= 8; ++file) {
//       uint64_t mask = squares_to_ULL(static_cast<Square>(file + rank * 8));
//       std::string piece;
//
//       if (b.white[KING] & mask)
//         piece = "♚";
//       else if (b->white[QUEEN] & mask)
//         piece = "♛";
//       else if (b->white[ROOK] & mask)
//         piece = "♜";
//       else if (b->white[KNIGHT] & mask)
//         piece = "♞";
//       else if (b->white[BISHOP] & mask)
//         piece = "♝";
//       else if (b->white[PAWN] & mask)
//         piece = "♟";
//       else if (b->black[king] & mask)
//         piece = "♔";
//       else if (b->black[queen] & mask)
//         piece = "♕";
//       else if (b->black[rook] & mask)
//         piece = "♖";
//       else if (b->black[knight] & mask)
//         piece = "♘";
//       else if (b->black[bishop] & mask)
//         piece = "♗";
//       else if (b->black[pawn] & mask)
//         piece = "♙";
//       else
//         piece = ".";
//
//       std::cout << piece << " ";
//     }
//     std::cout << rank << "\n";
//   }
//   std::cout << "  a b c d e f g h\n";
// }
