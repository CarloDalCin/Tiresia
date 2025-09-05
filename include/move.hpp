#pragma once

#include <cstdint>

enum class Square : uint8_t { // clang-format off
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
  SQUARE_NONE
}; // clang-format on

template <typename... Squares>
constexpr uint64_t squares_to_ULL(Squares... square) {
  return (0ULL | ... | (1ULL << static_cast<int>(square)));
}

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
    NORMAL = 0,           // just a normal move
    PROMOTION_KNIGHT = 1, // promotion to knight
    PROMOTION_BISHOP = 2, // promotion to bishop
    PROMOTION_ROOK = 3,   // ...to rook
    PROMOTION_QUEEN = 4,  // ...to queen
    EN_PASSANT = 5,       // en passant capture
    CASTLING = 6,         // castling (king side or quek side)

    // double pawn move (only valid for the first move of the pawn)
    DOUBLE_PAWN_PUSH = 7,
    CAPTURE = 8,
    CHECK = 9,
    CHECKMATE = 10,
    BULL_MOVE = 11, // for pruning

    // reserved for future use
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
