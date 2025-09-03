#pragma once

#include <cstdint>

enum Square { // clang-format off
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

// Move is only a wrapper for the information needed to move a piece it does not
// actually move the piece!! It contains information about the source and
// destination squares as well as flags
class Move {
private:
  // structure made of:
  // from (6 bits)
  // to (6 bits)
  // promotion (1 bit)
  // en passant (1 bit)
  // castling (1 bit)
  // ...
  union {
    uint16_t data; // packed data
    struct {
      uint8_t from : 6;       // from square (from 0 to 63)
      uint8_t to : 6;         // to square (from 0 to 63)
      uint8_t promotion : 1;  // promotion flag
      uint8_t en_passant : 1; // en passant flag
      uint8_t castling : 1;   // castling flag
      uint8_t reserved : 1;   // reserved
    };
  };

public:
  Move(uint8_t from, uint8_t to);
  //....
};
