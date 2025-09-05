#include "board.hpp"

#include <array>
#include <iostream>

Board Board::init_std() {
  Board b{};

  using enum Piece::Type;
  using enum Square;
  b.white[PAWN] = 0xFFULL << 8;
  b.black[PAWN] = 0xFFULL << 48;

  b.white[KNIGHT] = squares_to_ULL(B1, G1);
  b.black[KNIGHT] = squares_to_ULL(B8, G8);

  b.white[BISHOP] = squares_to_ULL(C1, F1);
  b.black[BISHOP] = squares_to_ULL(C8, F8);

  b.white[ROOK] = squares_to_ULL(A1, H1);
  b.black[ROOK] = squares_to_ULL(A8, H8);

  b.white[QUEEN] = squares_to_ULL(D1);
  b.black[QUEEN] = squares_to_ULL(D8);

  b.white[KING] = squares_to_ULL(E1);
  b.black[KING] = squares_to_ULL(E8);

  b.white[0] = b.white[PAWN] | b.white[KNIGHT] | b.white[BISHOP] |
               b.white[ROOK] | b.white[QUEEN] | b.white[KING];
  b.black[0] = b.black[PAWN] | b.black[KNIGHT] | b.black[BISHOP] |
               b.black[ROOK] | b.black[QUEEN] | b.black[KING];

  b.update_board_from_bitboards();
  return b;
}

Board Board::init_960() {
  Board b{};

  // Genera disposizione back rank Chess960
  std::array<int, 8> squares = {0, 1, 2, 3, 4, 5, 6, 7};

  // Fischer-Random:
  // https://en.wikipedia.org/wiki/Fischer%E2%80%93Random_chess_move_generation

  // ... TODO

  b.update_board_from_bitboards();
  return b;
}

std::string Board::get_ascii_piece(const Piece &piece) {
  static const char pieceChars[2][Piece::Type::PIECE_NB] = {
      {'.', 'P', 'N', 'B', 'R', 'Q', 'K'}, // White
      {'.', 'p', 'n', 'b', 'r', 'q', 'k'}  // Black
  };
  return std::string{pieceChars[piece.color()][piece.type()]};
}

std::string Board::get_utf8_piece(const Piece &piece) {
  static const std::string pieceUTF8[2][Piece::Type::PIECE_NB] = {
      {".", "♟", "♞", "♝", "♜", "♛", "♚"}, // White
      {".", "♙", "♘", "♗", "♖", "♕", "♔"}  // Black
  };
  return pieceUTF8[piece.color()][piece.type()];
}

void Board::print(std::string get_piece(const Piece &piece)) const {
  for (int rank = 7; rank >= 0; --rank) {
    std::cout << rank + 1 << " ";
    for (int file = 0; file < 8; ++file) {
      int index = rank * 8 + file;
      const auto &piece = board[index];
      std::string str = get_piece(piece);
      std::cout << str << " ";
    }
    std::cout << "\n";
  }
  std::cout << "  a b c d e f g h\n";
}
