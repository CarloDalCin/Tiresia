#pragma once

#include <array>
#include <cstdint>

//
//  Piece      = 0bCCTTTTTT
//  Color      = 0bCC
//  Type       = 0bTT
//

class Piece {
public:
  enum Color : uint8_t {
    WHITE = 0b00,
    BLACK = 0b01,
    COLOR_NB = 0b10,
    NO_COLOR = 0b11
  };

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

  // operator| for combining color and type
  friend constexpr Piece operator|(Color c, Type t) { return Piece(c, t); }
  friend constexpr Piece operator|(Type t, Color c) { return Piece(c, t); }

  // Factory functions
  constexpr explicit Piece(uint8_t ct) : data(ct) {}
  constexpr explicit Piece(Color c, Type t)
      : data((static_cast<uint8_t>(c) << 6) | static_cast<uint8_t>(t)) {}
  constexpr explicit Piece(Type t, Color c) : Piece(c, t) {}
  constexpr explicit Piece(const char c) {
    switch (c) { // clang-format off
    case 'p': data = PAWN   | WHITE; break;
    case 'P': data = PAWN   | BLACK; break;
    case 'n': data = KNIGHT | WHITE; break;
    case 'N': data = KNIGHT | BLACK; break;
    case 'b': data = BISHOP | WHITE; break;
    case 'B': data = BISHOP | BLACK; break;
    case 'r': data = ROOK   | WHITE; break;
    case 'R': data = ROOK   | BLACK; break;
    case 'q': data = QUEEN  | WHITE; break;
    case 'Q': data = QUEEN  | BLACK; break;
    case 'k': data = KING   | WHITE; break;
    case 'K': data = KING   | BLACK; break;
    default:  data = NO_PIECE | NO_COLOR; break;
    } // clang-format on
  }
  constexpr Piece() : data(Color::NO_COLOR | Type::NO_PIECE) {}
  static constexpr Piece empty() { return Piece(); }
  static constexpr Piece make(Color c, Type t) { return Piece(c | t); }
  static constexpr Piece make(Type t, Color c) { return Piece(c | t); }
  static constexpr Piece from(const uint8_t &ct) { return Piece(ct); }
  static constexpr Piece from(const char c) { return Piece(c); }

  // Conversion
  constexpr operator uint8_t() const { return data; }
  explicit constexpr operator bool() const { return type() != Type::NO_PIECE; }

  // Accessories
  constexpr Color color() const {
    return static_cast<Color>((data >> 6) & 0b11);
  }
  constexpr Type type() const { return static_cast<Type>(data & 0b00111111); }

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

  constexpr char to_letter() const {
    char l;
    switch (data) { // clang-format off
    case PAWN   | WHITE: l = 'P'; break;
    case PAWN   | BLACK: l = 'p'; break;
    case KNIGHT | WHITE: l = 'N'; break;
    case KNIGHT | BLACK: l = 'n'; break;
    case BISHOP | WHITE: l = 'B'; break;
    case BISHOP | BLACK: l = 'b'; break;
    case ROOK   | WHITE: l = 'R'; break;
    case ROOK   | BLACK: l = 'r'; break;
    case QUEEN  | WHITE: l = 'Q'; break;
    case QUEEN  | BLACK: l = 'q'; break;
    case KING   | WHITE: l = 'K'; break;
    case KING   | BLACK: l = 'k'; break;
    case NO_PIECE | NO_COLOR: l = '.'; break;
    default: l = '?'; break;
    } // clang-format on
    return l;
  }

private:
  uint8_t data;
};
