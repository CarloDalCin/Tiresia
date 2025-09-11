#pragma once

#include <cstdint>
#include <string>

class CastleRights {
public:
  enum Value : uint8_t {
    NONE = 0,
    WHITE_KINGSIDE = 1 << 0,
    WHITE_QUEENSIDE = 1 << 1,
    BLACK_KINGSIDE = 1 << 2,
    BLACK_QUEENSIDE = 1 << 3,

    WHITE_CASTLING = WHITE_KINGSIDE | WHITE_QUEENSIDE,
    BLACK_CASTLING = BLACK_KINGSIDE | BLACK_QUEENSIDE,
    ALL = WHITE_CASTLING | BLACK_CASTLING,
  };

  friend constexpr CastleRights operator|(Value lhs, Value rhs) {
    return static_cast<CastleRights>(static_cast<uint8_t>(lhs) |
                                     static_cast<uint8_t>(rhs));
  }

  friend constexpr CastleRights operator&(Value lhs, Value rhs) {
    return static_cast<CastleRights>(static_cast<uint8_t>(lhs) &
                                     static_cast<uint8_t>(rhs));
  }

  friend constexpr CastleRights operator~(Value rhs) {
    return static_cast<CastleRights>(~static_cast<uint8_t>(rhs));
  }

  constexpr CastleRights() : data(NONE) {}
  constexpr explicit CastleRights(Value v) : data(v) {}
  constexpr CastleRights(const CastleRights &v) : data(v.data) {}
  constexpr operator uint8_t() const { return static_cast<uint8_t>(data); }
  inline explicit CastleRights(const std::string &str) {
    for (auto c : str) {
      switch (c) { // clang-format off
        case 'K': data |= WHITE_KINGSIDE; break;
        case 'Q': data |= WHITE_QUEENSIDE; break;
        case 'k': data |= BLACK_KINGSIDE; break;
        case 'q': data |= BLACK_QUEENSIDE; break;
        default: data |= NONE; break;
      } // clang-format on
    }
  }

  // Factory functions
  static constexpr CastleRights none() { return CastleRights(NONE); }
  static constexpr CastleRights all() { return CastleRights(ALL); }
  static constexpr CastleRights make(Value v) { return CastleRights(v); }
  static inline CastleRights from(const std::string &str) {
    return CastleRights(str);
  }

private:
  uint8_t data; // 1 byte

  constexpr explicit CastleRights(uint8_t v) : data(v) {}
};
