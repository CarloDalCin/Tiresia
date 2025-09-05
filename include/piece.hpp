#pragma once

#include <array>
#include <cstdint>

//
//  ColorType  = 0bCCTTTTTT
//  Color      = 0bCC
//  Type       = 0bTT
//

class Piece {
public:
  enum Color : uint8_t { WHITE = 0b00, BLACK = 0b01, NO_COLOR = 0b11 };

  enum Type : uint8_t {
    NO_PIECE = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6,
    PIECE_NB
  };

  struct ColorType {
    uint8_t data;

    constexpr ColorType() : data(0) {}
    constexpr explicit ColorType(uint8_t d) : data(d) {}
    constexpr ColorType(Color c, Type t)
        : data((static_cast<uint8_t>(c) << 6) | static_cast<uint8_t>(t)) {}

    constexpr operator uint8_t() const { return data; }
    constexpr bool operator==(const ColorType &other) const {
      return data == other.data;
    }
    constexpr bool operator!=(const ColorType &other) const {
      return data != other.data;
    }
    constexpr Color color() const {
      return static_cast<Color>((data >> 6) & 0b11);
    }
    constexpr Type type() const { return static_cast<Type>(data & 0b00111111); }
  };

  // operator| for combining color and type
  friend constexpr ColorType operator|(Color c, Type t) {
    return ColorType(c, t);
  }
  friend constexpr ColorType operator|(Type t, Color c) {
    return ColorType(c, t);
  }

  // Factory functions
  constexpr Piece() : piece(ColorType{}) {}
  static constexpr Piece init(Color c, Type t) { return Piece(c | t); }
  static constexpr Piece init(Type t, Color c) { return Piece(c | t); }
  static constexpr Piece from(const ColorType &ct) { return Piece(ct); }
  static constexpr Piece empty() { return Piece(ColorType()); }

  // Conversion
  constexpr operator ColorType() const { return piece; }
  explicit constexpr operator bool() const { return type() != Type::NO_PIECE; }

  // Accessories
  constexpr Color color() const { return piece.color(); }
  constexpr Type type() const { return piece.type(); }

  constexpr bool is_empty() const { return type() == Type::NO_PIECE; }
  constexpr bool is_pawn() const { return type() == Type::PAWN; }
  constexpr bool is_knight() const { return type() == Type::KNIGHT; }
  constexpr bool is_bishop() const { return type() == Type::BISHOP; }
  constexpr bool is_rook() const { return type() == Type::ROOK; }
  constexpr bool is_queen() const { return type() == Type::QUEEN; }
  constexpr bool is_king() const { return type() == Type::KING; }

  constexpr bool is_white() const { return color() == Color::WHITE; }
  constexpr bool is_black() const { return color() == Color::BLACK; }

  // Comparison
  constexpr bool operator==(Type t) const { return type() == t; }
  constexpr bool operator!=(Type t) const { return type() != t; }
  constexpr bool operator==(Color c) const { return color() == c; }
  constexpr bool operator!=(Color c) const { return color() != c; }

  // Value of the piece
  constexpr uint8_t value() const {
    static constexpr std::array<uint8_t, Type::PIECE_NB> values{0, 1, 3, 3,
                                                                5, 9, 0};
    return values[static_cast<std::size_t>(type())];
  }

private:
  ColorType piece;

  // Costruttori privati
  constexpr explicit Piece(ColorType ct) : piece(ct) {}
};
