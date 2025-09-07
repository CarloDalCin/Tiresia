#pragma once

#include <cstdint>
#include <string>
#include <vector>

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
  std::array<Piece, 64> board; // 64 * 8 bits = 512 bits
  CastleRights castleRights;   // 8 bits + 56 bits(padding) = 64 bits
  union {                      // 2 * 7 * 64 bits = 896 bits
    std::array<std::array<uint64_t, Piece::Type::PIECE_NB>,
               Piece::Color::COLOR_NB>
        pieces; // 0 = white, 1 = black
    struct {
      // index 0 = all pieces of the same color the union of all white or
      // black pieces
      std::array<uint64_t, Piece::Type::PIECE_NB> white;
      std::array<uint64_t, Piece::Type::PIECE_NB> black;
    };
  };

public:
  // Constructors
  constexpr explicit Board()
      : board{}, castleRights(CastleRights::NO_CASTLE), pieces{{}} {}
  constexpr Board(const Board &b) = default;
  constexpr Board(const std::string &fen);

  // Factory functions
  static constexpr Board empty() { return Board(); }
  static constexpr Board from_fen(const std::string &fen) { return Board(fen); }

  // Conversion
  Board &operator=(const Board &b) = default;

  constexpr void set_piece(Square sq, Piece p) {
    const int idx = static_cast<int>(sq);
    // Remove old piece from bitboard
    const Piece old = board[idx];
    if (old)
      pieces[old.color()][static_cast<uint8_t>(old.type())] &= ~(1ULL << idx);

    // Insert new piece into bitboard
    if (p)
      pieces[p.color()][static_cast<uint8_t>(p.type())] |= (1ULL << idx);

    // Update Piece array
    board[idx] = p;
  }

  constexpr Piece get_piece(Square sq) const {
    return board[static_cast<int>(sq)];
  }

  // Aggiorna l’array board[64] a partire dalle bitboard
  constexpr void update_board_from_bitboards() {
    // Clear board
    for (int i = 0; i < 64; ++i)
      board[i] = Piece::empty();

    // Update from bitboard white
    for (uint8_t t = 1; t < Piece::Type::PIECE_NB; ++t) {
      uint64_t bits = white[t];
      for (int sq = 0; bits; ++sq) {
        if (bits & 1ULL)
          board[sq] =
              Piece::make(Piece::Color::WHITE, static_cast<Piece::Type>(t));
        bits >>= 1;
      }
    }

    // update from bitboard black
    for (uint8_t t = 1; t < Piece::Type::PIECE_NB; ++t) {
      uint64_t bits = black[t];
      for (int sq = 0; bits; ++sq) {
        if (bits & 1ULL)
          board[sq] =
              Piece::make(Piece::Color::BLACK, static_cast<Piece::Type>(t));
        bits >>= 1;
      }
    }
  }

  // Move piece 'from' to 'to' with Piece board
  constexpr void move_piece(Square from, Square to) {
    const Piece p = get_piece(from);
    set_piece(from, Piece::empty());
    set_piece(to, p);
  }

  // Move piece 'from' to 'to' with bitboards
  constexpr void move_piece_bitboard(Square from, Square to, Piece ct) {
    const int from_idx = static_cast<int>(from);
    const int to_idx = static_cast<int>(to);
    Piece::Color color = ct.color();
    Piece::Type type = ct.type();

    // Remove piece from old position
    pieces[color][type] &= ~(1ULL << from_idx);

    // Add piece to new position
    pieces[color][type] |= (1ULL << to_idx);

    update_board_from_bitboards();
  }

  static std::string get_utf8_piece(const Piece &piece);
  static std::string get_ascii_piece(const Piece &piece);

  void inline print(std::string (*)(const Piece &)) const;

public:
  // Factory functions
  static constexpr Board init_std();
  static constexpr Board init_960();
};
