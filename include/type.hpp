#pragma once

#include <cstdint>

//  color_t
//    |
// 0b 00 000000
//       |
//     piece_t
//

enum class piece_t : uint8_t {
    empty = 0b00_000000,
    pawn = 0b00_000001,
    knight = 0b00_000010,
    bishop = 0b00_000011,
    rook = 0b00_000100,
    queen = 0b00_000101,
    king = 0b00_000110,
};

enum class color_t : uint8_t {
    empty = 0b00_000000,
    white = 0b01_000000,
    black = 0b10_000000,
};

enum class castle_t : uint8_t {
    king = 0,
    queen = 1,
};

constexpr castle_t O_O = castle_t::king;
constexpr castle_t O_O_O = castle_t::queen;

using colored_piece = uint8_t;

inline colored_piece make_piece(color_t c, piece_t p) {
    return static_cast<uint8_t>(c) | static_cast<uint8_t>(p);
}

piece_t getPiece(uint8_t v) {
    return static_cast<piece_t>(v & 0b00111111); // ultimi 6 bit
}

color_t getColor(uint8_t v) {
    return static_cast<color_t>(v & 0b11000000); // primi 2 bit
}
