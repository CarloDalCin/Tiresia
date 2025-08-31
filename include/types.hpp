#pragma once

#include <cstdint>

//  color_t
//    |
// 0b 00 000000
//       |
//     piece_t
//

struct move_t {
    uint64_t from;
    uint64_t to;
};

// for boath color and piece
constexpr uint64_t empty = 0b00000000;

enum piece_t : uint8_t {
    pawn = 0b00000001,
    knight = 0b00000010,
    bishop = 0b00000011,
    rook = 0b00000100,
    queen = 0b00000101,
    king = 0b00000110,
    PIECE_NB,
};
constexpr uint64_t all = empty; // index to 0 so that can be used for bitboards indexing

enum color_t : uint8_t {
    white = 0b01000000,
    black = 0b10000000,
};

//  castle type for defining where the king want to go
//  may be imporoved the logic for considering where the king can go
enum class castle_t : uint8_t {
    king = 0,
    queen = 1,
};
constexpr castle_t O_O = castle_t::king; // short castle
constexpr castle_t O_O_O = castle_t::queen; // long castle

// type for boath color and piece
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
