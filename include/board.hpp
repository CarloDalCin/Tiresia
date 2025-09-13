#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <regex>
#include <string>

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
  // total size = 1408 bits = 176 bytes
  std::array<Piece, 64> mailbox; // 64 * 8 bits = 512 bits
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
  constexpr explicit Board() : mailbox{}, pieces{{}} {}
  constexpr Board(const Board &b) = default;
  // FEN ref: https://it.wikipedia.org/wiki/Notazione_Forsyth-Edwards
  inline Board(const std::string &fen) { set_from_fen(fen); }

  // Factory functions
  static constexpr Board empty() { return Board(); }
  static inline Board from_fen(const std::string &fen) { return Board(fen); }

  // Conversion
  inline Board &operator=(const Board &b) = default;

  // Get piece at square sq from the mailbox (technically more efficient than
  // get_piece_in_bitboard_at)
  constexpr Piece get_piece_in_mailbox_at(Square sq) const {
    return mailbox.at(static_cast<int>(sq));
  }

  // Get piece at square sq from the bitboard (the only useful case is when you
  // want to use the bitboard a lot by improving the use of the cache) (((maybe,
  // sould be tested!)))
  // TODO: check if it's worth it in some cases
  constexpr Piece get_piece_in_bitboard_at(Square sq) const {
    for (int i = 1; i < Piece::Type::PIECE_NB; ++i) {
      if (pieces.at(Piece::Color::WHITE).at(i) & Square::to_uint64(sq)) {
        return Piece(Piece::Color::WHITE, static_cast<Piece::Type>(i));
      }
      if (pieces.at(Piece::Color::BLACK).at(i) & Square::to_uint64(sq)) {
        return Piece(Piece::Color::BLACK, static_cast<Piece::Type>(i));
      }
    }
    return Piece::empty();
  }

  // Set piece at square sq
  // Note do not use set_piece(sq) instead of remove_piece(sq)
  constexpr void set_piece(Square to, Piece p = Piece::empty()) {
    if (p) [[likely]] {
      mailbox.at(static_cast<int>(to)) = p;
      pieces.at(p.color()).at(p.type()) |= Square::to_uint64(to);
    } else [[unlikely]] {
      remove_piece(to);
    }
  }

  // Remove piece at square sq
  // Note: do not use set_piece(sq) instead of remove_piece(sq)
  constexpr void remove_piece(Square sq) {
    Piece p = get_piece_in_mailbox_at(sq);
    if (p) {
      mailbox.at(static_cast<int>(sq)) = Piece::empty();
      pieces.at(p.color()).at(p.type()) &= ~(Square::to_uint64(sq));
    }
  }

  // Get the available moves for a piece at square sq
  // TODO
  inline std::vector<Move> get_moves_for_piece_at(Square sq) const {
    std::vector<Move> moves;
    const Piece p = get_piece_in_bitboard_at(sq);
    if (p) {
      switch (p.type()) {
      case Piece::Type::PAWN:
      case Piece::Type::KNIGHT:
      case Piece::Type::BISHOP:
      case Piece::Type::ROOK:
      case Piece::Type::QUEEN:
      case Piece::Type::KING:
        break;
      }
    }
    return moves;
  }

  // Move piece 'from' to 'to'
  constexpr void move_piece(Square from, Square to) {
    const Piece p = get_piece_in_mailbox_at(from);
    if (p) [[likely]] {
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
        {".", "♙", "♘", "♗", "♖", "♕", "♔"}, // White
        {".", "♟", "♞", "♝", "♜", "♛", "♚"}, // Black
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
    return Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  }

  // TODO
  // Fischer-Random:
  // https://en.wikipedia.org/wiki/Fischer%E2%80%93Random_chess_move_generation
  // for now it's a random position for tests purposes
  static inline Board init_960() {
    return Board("nqrkrbbn/pppppppp/8/8/8/8/PPPPPPPP/NQRKRBBN");
  }

  // only modify the pieces it doesn't consider the other fields of FEN
  // position
  inline void set_from_fen(const std::string &fen) {
    auto match = fen_validator(fen);
    if (!match)
      throw std::runtime_error("Invalid FEN");

    clear();

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
  static inline std::optional<std::smatch>
  fen_validator(const std::string &fen) {
    static const std::regex fen_regex(
        R"(^((?:[pnbrqkPNBRQK1-8]+/){7}[pnbrqkPNBRQK1-8]+))" // position
        R"(.*)" // rest not considered
    );
    std::smatch match;
    if (!std::regex_match(fen, match, fen_regex)) {
      return std::nullopt; // no match → FEN invalid
    }
    return match; // retrurn all the match groups
  }

  // Clear the board
  constexpr void clear() {
    for (int i = 0; i < mailbox.size(); ++i)
      remove_piece(static_cast<Square>(i));
  }

  // check if 2 squares are ocuppied by the same color
  constexpr bool are_in_the_same_team(Square p1, Square p2) const {
    return get_piece_in_mailbox_at(p1).color() ==
           get_piece_in_mailbox_at(p2).color();
  }
};
