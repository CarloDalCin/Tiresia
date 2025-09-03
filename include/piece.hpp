#pragma once

#include <cstdint>

//
//  Piece      = 0bCCTTTTTT
//  Color      = 0bCC
//  PieceType  = 0bTT
//

enum Color : uint8_t { NO_COLOR = 0, WHITE = 0b01000000, BLACK = 0b10000000 };

enum PieceType : uint8_t {
  NO_PIECE = 0,
  PAWN = 0b00000001,
  KNIGHT = 0b00000010,
  BISHOP = 0b00000011,
  ROOK = 0b00000100,
  QUEEN = 0b00000101,
  KING = 0b00000110,
};

static inline Color operator|(Color color, PieceType type) {
  return static_cast<Color>(static_cast<uint8_t>(color) |
                            static_cast<uint8_t>(type));
}

class Piece {
public:
  enum Value : uint8_t {
    EMPTY = 0,
    // white
    W_PAWN = 0b01000001,
    W_KNIGHT = 0b01000010,
    W_BISHOP = 0b01000011,
    W_ROOK = 0b01000100,
    W_QUEEN = 0b01000101,
    W_KING = 0b01000110,
    // black
    B_PAWN = 0b10000001,
    B_KNIGHT = 0b10000010,
    B_BISHOP = 0b10000011,
    B_ROOK = 0b10000100,
    B_QUEEN = 0b10000101,
    B_KING = 0b10000110,
  };

  Piece() = default;
  constexpr Piece(Value piece) : piece(piece) {}
  constexpr Piece(Color color, PieceType type)
      : piece(static_cast<Value>(color | type)) {}

  // implicit conversion ito Value for switch and comparison
  constexpr operator Value() const { return piece; }
  // the use of 'if(piece)' will be false for an empty piece
  explicit constexpr operator bool() const { return piece != EMPTY; }

  constexpr Color color() const;    // return the color of the piece
  constexpr PieceType type() const; // return the type of the piece

  constexpr bool is_empty() const;
  constexpr bool is_pawn() const;
  constexpr bool is_knight() const;
  constexpr bool is_bishop() const;
  constexpr bool is_rook() const;
  constexpr bool is_queen() const;
  constexpr bool is_king() const;

  constexpr bool is_white() const;
  constexpr bool is_black() const;

  constexpr bool operator==(const Value &other) const { return piece == other; }
  constexpr bool operator!=(const Value &other) const { return piece != other; }
  constexpr bool operator==(const PieceType &other) const {
    return type() == other;
  }
  constexpr bool operator!=(const PieceType &other) const {
    return type() != other;
  }
  constexpr bool operator==(const Color &other) const {
    return color() == other;
  }
  constexpr bool operator!=(const Color &other) const {
    return color() != other;
  }

  // return the value of the piece
  // if the piece is empty, return 0
  constexpr uint8_t value() const;

private:
  Value piece;
};
