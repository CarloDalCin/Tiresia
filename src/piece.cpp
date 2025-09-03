#include "piece.hpp"

constexpr Color Piece::color() const {
  return static_cast<Color>((piece & 0b11000000) >> 6);
}

constexpr PieceType Piece::type() const {
  return static_cast<PieceType>(piece & 0b00111111);
}

constexpr bool Piece::is_empty() const { return piece == EMPTY; }
constexpr bool Piece::is_pawn() const { return type() == PAWN; }
constexpr bool Piece::is_knight() const { return type() == KNIGHT; }
constexpr bool Piece::is_bishop() const { return type() == BISHOP; }
constexpr bool Piece::is_rook() const { return type() == ROOK; }
constexpr bool Piece::is_queen() const { return type() == QUEEN; }
constexpr bool Piece::is_king() const { return type() == KING; }

constexpr bool Piece::is_white() const { return color() == WHITE; }
constexpr bool Piece::is_black() const { return color() == BLACK; }

// return the value of the piece
// if the piece is empty, return 0
constexpr uint8_t Piece::value() const {
  static constexpr uint8_t values[] = {
      [NO_PIECE] = 0, [PAWN] = 1,  [KNIGHT] = 3, [BISHOP] = 3,
      [ROOK] = 5,     [QUEEN] = 9, [KING] = 0,
  };
  return values[type()];
}
