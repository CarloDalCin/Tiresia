#include "move.hpp"

constexpr Square Move::get_from() const {
  return static_cast<Square>((data & FROM_MASK) >> FROM_SHIFT);
}

constexpr Square Move::get_to() const {
  return static_cast<Square>((data & TO_MASK) >> TO_SHIFT);
}

constexpr Move::Type Move::get_type() const {
  return static_cast<Move::Type>((data & TYPE_MASK) >> TYPE_SHIFT);
}

constexpr void Move::set_from(Square from) {
  data &= ~FROM_MASK;
  data |= (static_cast<uint16_t>(from) << FROM_SHIFT) & FROM_MASK;
}

constexpr void Move::set_to(Square to) {
  data &= ~TO_MASK;
  data |= (static_cast<uint16_t>(to) << TO_SHIFT) & TO_MASK;
}

constexpr void Move::set_type(Move::Type type) {
  data &= ~TYPE_MASK;
  data |= (static_cast<uint16_t>(type) << TYPE_SHIFT) & TYPE_MASK;
}
