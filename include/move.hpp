#pragma once

#include <cstdint>
#include <string>

class Square {
public:
  enum Value : uint8_t { // clang-format off
//0   1   2   3   4   5   6   7
  A1, B1, C1, D1, E1, F1, G1, H1,
//8   9   10  11  12  13  14  15
  A2, B2, C2, D2, E2, F2, G2, H2,
//16  17  18  19  20  21  22  23
  A3, B3, C3, D3, E3, F3, G3, H3,
//24  25  26  27  28  29  30  31
  A4, B4, C4, D4, E4, F4, G4, H4,
//32  33  34  35  36  37  38  39
  A5, B5, C5, D5, E5, F5, G5, H5,
//40  41  42  43  44  45  46  47
  A6, B6, C6, D6, E6, F6, G6, H6,
//48  49  50  51  52  53  54  55
  A7, B7, C7, D7, E7, F7, G7, H7,
//56  57  58  59  60  61  62  63
  A8, B8, C8, D8, E8, F8, G8, H8,
  NONE
}; // clang-format on

  constexpr explicit Square() : square(Value::NONE) {}
  constexpr explicit Square(Value square) : square(square) {}
  constexpr Square(uint8_t square) : square(static_cast<Value>(square)) {}
  constexpr Square(const std::string &str) : square(Value::NONE) {
    int file = std::tolower(str[0]) - 'a'; // a=0, b=1, ...
    int rank = str[1] - '1';               // 1=0, 2=1, ..., 8=7
    square = static_cast<Value>(rank * 8 + file);
  }
  static constexpr Square from(const std::string &str) { return Square(str); }
  static constexpr Square from(Value square) { return Square(square); }

  constexpr uint8_t to_int() const { return static_cast<uint8_t>(square); }
  constexpr operator uint8_t() const { return to_int(); }

  friend constexpr Square operator|(Square lhs, Square rhs) = delete;
  friend constexpr Square operator&(Square lhs, Square rhs) = delete;

  // TODO xor and not
  friend constexpr Square operator^(Square lhs, Square rhs) = delete;
  friend constexpr Square operator~(Square lhs) = delete;

  // Convert a list of squares to a uint64_t
  template <typename... Squares>
  static constexpr uint64_t to_uint64(Squares... square) {
    return (0ULL | ... | (1ULL << static_cast<int>(square)));
  }

private:
  Value square;
};

// Move is only a wrapper for the information needed to move a piece, it does
// not actually move the piece!! It contains information about the source and
// destination squares as well as flags
class Move {
private:
  // structure made of:
  // from (6 bits)
  // to (6 bits)
  // move type (4 bits)
  uint16_t data; // packed data

  static constexpr uint16_t FROM_MASK = 0x003F; // 6 bits (0000 0000 0011 1111)
  static constexpr uint16_t TO_MASK = 0x0FC0;   // 6 bits (0000 1111 1100 0000)
  static constexpr uint16_t TYPE_MASK = 0xF000; // 4 bits (1111 0000 0000 0000)

  static constexpr uint16_t FROM_SHIFT = 0;
  static constexpr uint16_t TO_SHIFT = 6;
  static constexpr uint16_t TYPE_SHIFT = 12;

public:
  enum class Type : uint8_t {
    NORMAL = 0, // just a normal move
    CASTLING = 1,

    // promotion_piece = piece.type()
    // example: promotion_KNIGHT = Piece::Type::KNIGHT
    PROMOTION_KNIGHT = 2, // promotion to knight
    PROMOTION_BISHOP = 3, // promotion to bishop
    PROMOTION_ROOK = 4,   // ...to rook
    PROMOTION_QUEEN = 5,  // ...to queen
    EN_PASSANT = 6,       // en passant capture
    // double pawn move (only valid for the first move of the pawn)
    DOUBLE_PAWN_PUSH = 7,
    BULL_MOVE = 8, // for pruning

    // reserved for future use
    RESERVED_9 = 9,
    RESERVED_10 = 10,
    RESERVED_11 = 11,
    RESERVED_12 = 12,
    RESERVED_13 = 13,
    RESERVED_14 = 14,
    RESERVED_15 = 15
  };

  Move() = delete;
  constexpr explicit Move(Square from, Square to, Type type = Type::NORMAL)
      : data(0) {
    set_from(from);
    set_to(to);
    set_type(type);
  }

  constexpr bool operator==(const Type type) const {
    return this->type() == type;
  }

  constexpr bool operator!=(const Type type) const {
    return this->type() != type;
  }

  constexpr Square from() const {
    return static_cast<Square>((data & FROM_MASK) >> FROM_SHIFT);
  }

  constexpr Square to() const {
    return static_cast<Square>((data & TO_MASK) >> TO_SHIFT);
  }

  constexpr Type type() const {
    return static_cast<Move::Type>((data & TYPE_MASK) >> TYPE_SHIFT);
  }

private:
  constexpr void set_from(Square from) {
    data &= ~FROM_MASK;
    data |= (static_cast<uint16_t>(from) << FROM_SHIFT) & FROM_MASK;
  }

  constexpr void set_to(Square to) {
    data &= ~TO_MASK;
    data |= (static_cast<uint16_t>(to) << TO_SHIFT) & TO_MASK;
  }

  constexpr void set_type(Move::Type type) {
    data &= ~TYPE_MASK;
    data |= (static_cast<uint16_t>(type) << TYPE_SHIFT) & TYPE_MASK;
  }
};
