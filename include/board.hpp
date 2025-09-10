#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <regex>
#include <string>

#include "castle.hpp"
#include "move.hpp"
#include "piece.hpp"

/*
The board is represented as a 64 bit integer, with each bit representing a
cells

  Board representation
| Casella | Bit index |
| ------- | --------- |
| a1      | 0         |
| b1      | 1         |
| ...     | ...       |
| h1      | 7         |
| a2      | 8         |
| ...     | ...       |
| h8      | 63        |

for a better understanding of how the board is represented see the move.hpp file
*/

class Board {
private:
  // total size = 1472 bits = 184 bytes
  std::array<Piece, 64> mailbox; // 64 * 8 bits = 512 bits
  CastleRights castleRights;     // 8 bits + 56 bits(padding) = 64 bits
  union {                        // 2 * 7 * 64 bits = 896 bits
    std::array<std::array<uint64_t, Piece::Type::PIECE_NB>,
               Piece::Color::COLOR_NB>
        pieces; // 0 = white, 1 = black
    struct {
      // index 0 = all pieces of the same color the union of all white or
      // black pieces
      std::array<uint64_t, Piece::Type::PIECE_NB> white;
      std::array<uint64_t, Piece::Type::PIECE_NB> black;
    };
  };

public:
  // Constructors
  constexpr explicit Board()
      : mailbox{}, castleRights(CastleRights::NO_CASTLE), pieces{{}} {}
  constexpr Board(const Board &b) = default;
  // FEN ref: https://it.wikipedia.org/wiki/Notazione_Forsyth-Edwards
  inline Board(const std::string &fen) { set_from_fen(fen); }

  // Factory functions
  static constexpr Board empty() { return Board(); }
  static inline Board from_fen(const std::string &fen) { return Board(fen); }

  // Conversion
  inline Board &operator=(const Board &b) = default;

  constexpr Piece get_piece(Square sq) const {
    return mailbox.at(static_cast<int>(sq));
  }

  // Set piece at square sq
  // Note do not use set_piece(sq) instead of remove_piece(sq)
  constexpr void set_piece(Square to, Piece p = Piece::empty()) {
    mailbox.at(static_cast<int>(to)) = p;
    if (p)
      pieces.at(p.color()).at(p.type()) |= squares_to_ULL(to);
  }

  // Remove piece at square sq
  // Note do not use set_piece(sq) instead of remove_piece(sq)
  constexpr void remove_piece(Square sq) {
    Piece p = get_piece(sq);
    if (p) {
      pieces.at(p.color()).at(p.type()) &= ~(squares_to_ULL(sq));
      set_piece(sq);
    }
  }

  // Move piece 'from' to 'to'
  constexpr void move_piece(Square from, Square to) {
    const Piece p = get_piece(from);
    if (p) {
      remove_piece(from);
      set_piece(to, p);
    }
  }

  // Overload move_piece with Move
  constexpr void move_piece(const Move &move) {
    move_piece(move.from(), move.to());
  }

  static inline std::string get_utf8_piece(const Piece &piece) {
    static const std::string pieceUTF8[2][Piece::Type::PIECE_NB] = {
        {".", "♟", "♞", "♝", "♜", "♛", "♚"}, // White
        {".", "♙", "♘", "♗", "♖", "♕", "♔"}  // Black
    };
    if (piece.is_empty())
      return pieceUTF8[0][0];
    return pieceUTF8[piece.color()][piece.type()];
  }

  static inline std::string get_ascii_piece(const Piece &piece) {
    return std::string{piece.to_letter()};
  }

  inline void print(std::string get_piece_rapresentation(const Piece &)) const {
    for (int rank = 7; rank >= 0; --rank) {
      std::printf("%d ", rank + 1);
      for (int file = 0; file < 8; ++file) {
        int index = rank * 8 + file;
        const auto &piece = mailbox[index];
        std::string str = get_piece_rapresentation(piece);
        std::printf("%s ", static_cast<const char *>(str.c_str()));
      }
      std::printf("\n");
    }
    std::printf("  a b c d e f g h\n");
  }

public:
  // Factory functions
  static inline Board init_std() {
    return Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  }

  // TODO
  // Fischer-Random:
  // https://en.wikipedia.org/wiki/Fischer%E2%80%93Random_chess_move_generation
  // for now it's a random position for tests purposes
  static inline Board init_960() {
    return Board("nqrkrbbn/pppppppp/8/8/8/8/PPPPPPPP/NQRKRBBN w KQkq - 0 1");
  }

  // only modify the pieces it doesn't consider the other fields of FEN
  // position
  inline void set_from_fen(const std::string &fen) {
    auto match = fen_validator(fen);
    if (!match)
      throw std::runtime_error("Invalid FEN");

    // Clear the board
    for (int i = 0; i < 64; ++i)
      set_piece(static_cast<Square>(i), Piece::empty());

    // Obtained the position part of separated by '/'
    std::string position = (*match)[1].str();
    size_t start = 0;
    int rank = 7; // FEN start from a8

    while (rank >= 0) {
      size_t end = position.find('/', start);
      std::string row;
      if (end == std::string::npos)
        row = position.substr(start); // last row
      else
        row = position.substr(start, end - start);

      int file = 0;
      for (char c : row) {
        if (isdigit(c)) {
          file += c - '0';
          if (file > 8)
            throw std::runtime_error("Invalid FEN: rank overflow");
        } else {
          if (file >= 8)
            throw std::runtime_error("Invalid FEN: file overflow");

          int index = rank * 8 + file;
          set_piece(static_cast<Square>(index), Piece(c));
          ++file;
        }
      }

      if (file != 8)
        throw std::runtime_error("Invalid FEN: rank does not sum to 8 squares");

      if (end == std::string::npos)
        break;
      start = end + 1;
      --rank;
    }
  }

  // TODO
  // this function only return the FEN position of the board and not the other
  // fields of FEN
  inline std::string to_fen() const { return {}; }

private:
  // function for validating a FEN position
  // match[0] is the whole match of the position
  // match[1] is the turn
  // match[2] is the castle rights
  // match[3] is the en passant square
  // match[4] is the halfmove
  // match[5] is the fullmove
  static inline std::optional<std::smatch>
  fen_validator(const std::string &fen) {
    static const std::regex fen_regex(
        R"(^((?:[pnbrqkPNBRQK1-8]+/){7}[pnbrqkPNBRQK1-8]+)\s)" // posizione
        R"((w|b)\s)"                                           // turno
        R"((-|K?Q?k?q?)\s)"                                    // arrocco
        R"((-|[a-h][36])\s)"                                   // en passant
        R"((\d+)\s(\d+)$)" // half move and full move
    );

    std::smatch match;
    if (!std::regex_match(fen, match, fen_regex)) {
      return std::nullopt; // no match → FEN invalid
    }

    // Every file must have cells
    std::string position = match[1];
    size_t start = 0, end;
    int rows = 0;
    while ((end = position.find('/', start)) != std::string::npos) {
      std::string row = position.substr(start, end - start);
      int count = 0;
      for (char c : row)
        count += isdigit(c) ? c - '0' : 1;
      if (count != 8)
        return std::nullopt;
      rows++;
      start = end + 1;
    }
    //
    {
      std::string row = position.substr(start);
      int count = 0;
      for (char c : row)
        count += isdigit(c) ? c - '0' : 1;
      if (count != 8)
        return std::nullopt;
      rows++;
    }

    if (rows != 8)
      return std::nullopt;

    return match; // retrurn all the match groups
  }
};
