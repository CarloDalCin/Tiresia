#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <regex>
#include <string>

#include "castle.hpp"
#include "move.hpp"
#include "piece.hpp"

#ifdef DEBUG
#define TODO                                                                   \
  retrun {}
#endif

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
  std::array<Piece, 64> board; // 64 * 8 bits = 512 bits
  CastleRights castleRights;   // 8 bits + 56 bits(padding) = 64 bits
  union {                      // 2 * 7 * 64 bits = 896 bits
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
      : board{}, castleRights(CastleRights::NO_CASTLE), pieces{{}} {}
  constexpr Board(const Board &b) = default;
  // FEN ref: https://it.wikipedia.org/wiki/Notazione_Forsyth-Edwards
  inline Board(const std::string &fen) { set_from_fen(fen); }

  // Factory functions
  static constexpr Board empty() { return Board(); }
  static inline Board from_fen(const std::string &fen) { return Board(fen); }

  // Conversion
  inline Board &operator=(const Board &b) = default;

  constexpr void set_piece(Square sq, Piece p) {
    const int idx = static_cast<int>(sq);
    // Remove old piece from bitboard
    const Piece old = board[idx];
    if (old)
      pieces[old.color()][static_cast<uint8_t>(old.type())] &= ~(1ULL << idx);

    // Insert new piece into bitboard
    if (p)
      pieces[p.color()][static_cast<uint8_t>(p.type())] |= (1ULL << idx);

    // Update Piece array
    board[idx] = p;
  }

  constexpr Piece get_piece(Square sq) const {
    return board[static_cast<int>(sq)];
  }

  // Aggiorna l’array board[64] a partire dalle bitboard
  constexpr void update_board_from_bitboards() {
    // Clear board
    for (int i = 0; i < 64; ++i)
      board[i] = Piece::empty();

    // Update from bitboard white
    for (uint8_t t = 1; t < Piece::Type::PIECE_NB; ++t) {
      uint64_t bits = white[t];
      for (int sq = 0; bits; ++sq) {
        if (bits & 1ULL)
          board[sq] =
              Piece::make(Piece::Color::WHITE, static_cast<Piece::Type>(t));
        bits >>= 1;
      }
    }

    // update from bitboard black
    for (uint8_t t = 1; t < Piece::Type::PIECE_NB; ++t) {
      uint64_t bits = black[t];
      for (int sq = 0; bits; ++sq) {
        if (bits & 1ULL)
          board[sq] =
              Piece::make(Piece::Color::BLACK, static_cast<Piece::Type>(t));
        bits >>= 1;
      }
    }
  }

  // Move piece 'from' to 'to' with Piece board
  constexpr void move_piece(Square from, Square to) {
    const Piece p = get_piece(from);
    set_piece(from, Piece::empty());
    set_piece(to, p);
  }

  // Move piece 'from' to 'to' with bitboards
  constexpr void move_piece_bitboard(Square from, Square to, Piece ct) {
    const int from_idx = static_cast<int>(from);
    const int to_idx = static_cast<int>(to);
    Piece::Color color = ct.color();
    Piece::Type type = ct.type();

    // Remove piece from old position
    pieces[color][type] &= ~(1ULL << from_idx);

    // Add piece to new position
    pieces[color][type] |= (1ULL << to_idx);

    update_board_from_bitboards();
  }

  static inline std::string get_utf8_piece(const Piece &piece) {
    static const std::string pieceUTF8[2][Piece::Type::PIECE_NB] = {
        {".", "♟", "♞", "♝", "♜", "♛", "♚"}, // White
        {".", "♙", "♘", "♗", "♖", "♕", "♔"}  // Black
    };
    return pieceUTF8[piece.color()][piece.type()];
  }
  static inline std::string get_ascii_piece(const Piece &piece) {
    static const char pieceChars[2][Piece::Type::PIECE_NB] = {
        {'.', 'P', 'N', 'B', 'R', 'Q', 'K'}, // White
        {'.', 'p', 'n', 'b', 'r', 'q', 'k'}  // Black
    };
    return std::string{pieceChars[piece.color()][piece.type()]};
  }

  inline void print(std::string get_piece(const Piece &)) const {
    for (int rank = 7; rank >= 0; --rank) {
      std::printf("%d ", rank + 1);
      for (int file = 0; file < 8; ++file) {
        int index = rank * 8 + file;
        const auto &piece = board[index];
        std::string str = get_piece(piece);
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
  // for now it's a random position for tests purposesO
  static inline Board init_960() {
    return Board("nqrkrbbn/pppppppp/8/8/8/8/PPPPPPPP/NQRKRBBN w KQkq - 0 1");
  }

  // only modify the pieces it doesn't consider the other fields of FEN position
  inline void set_from_fen(const std::string &fen) {
    auto match = fen_validator(fen);
    if (!match)
      throw std::runtime_error("Invalid FEN");

    // Set pieces for each row
    for (int i = 1; i < 9; ++i) {
      uint8_t file{};
      int rank = 8 - i; // fen start from a8

      for (char c : (*match)[i].str()) {
        if (isdigit(c)) {
          file += c - '0';
          continue;
        }
        if (isalpha(c)) {
          using enum Piece::Type;
          using enum Piece::Color;
          uint8_t p;
          switch (c) { // clang-format off
          case 'p': p = PAWN | BLACK; break;
          case 'P': p = PAWN | WHITE; break;
          case 'n': p = KNIGHT | BLACK; break;
          case 'N': p = KNIGHT | WHITE; break;
          case 'b': p = BISHOP | BLACK; break;
          case 'B': p = BISHOP | WHITE; break;
          case 'r': p = ROOK | BLACK; break;
          case 'R': p = ROOK | WHITE; break;
          case 'q': p = QUEEN | BLACK; break;
          case 'Q': p = QUEEN | WHITE; break;
          default:  p = Piece::empty(); break;
          } // clang-format on
          int index = rank * 8 + file;
          board.at(index) = Piece(p);
          ++file;
        }
      }
    }
  }

  // TODO
  inline std::string to_fen() const { return {}; }

private:
  // function for validating a FEN position
  // match[0] is the whole match of pieces = match[1-8]
  // match[1-8] are the 8 rows of the board
  // match[9] is the turn
  // match[10] is the castle rights
  // match[11] is the en passant square
  // match[12] is the halfmove
  // match[13] is the fullmove
  static inline std::optional<std::smatch>
  fen_validator(const std::string &fen) {
    static const std::regex fen_regex(
        R"(^((?:([pnbrqkPNBRQK1-8]+)/){7}([pnbrqkPNBRQK1-8]+))\s)" // posizione
        R"((w|b)\s)"                                               // turno
        R"((-|K?Q?k?q?)\s)"                                        // arrocco
        R"((-|[a-h][36])\s)"                                       // en passant
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
