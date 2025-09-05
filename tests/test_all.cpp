#include <cassert>
#include <iostream>

// Include le tue classi
#include "board.hpp"
#include "castle.hpp"
#include "libtiresia.hpp"
#include "move.hpp"
#include "piece.hpp"

int main(int argc, char **argv) {
  std::cout << "libtiresia test suite" << std::endl;
  std::cout << "TEST START ........................................"
            << std::endl;

  // Test Board
  Board b = Board::init_std();
  b.print(Board::get_utf8_piece);
  b.print(Board::get_ascii_piece);

  // Test Piece
  Piece w_pawn = Piece::init(Piece::Color::WHITE, Piece::Type::PAWN);
  Piece b_pawn = Piece::init(Piece::Color::BLACK, Piece::Type::PAWN);
  Piece w_rook = Piece::init(Piece::Color::WHITE, Piece::Type::ROOK);

  assert(w_pawn.is_white() && w_pawn.is_pawn());
  assert(b_pawn.is_black() && b_pawn.is_pawn());
  assert(w_rook.is_white() && w_rook.is_rook());

  // Test CastleRights
  CastleRights cr = CastleRights::ALL_CASTLING;

  // Test Move
  Move m(Square::A1, Square::A2, Move::Type::NORMAL);
  assert(m.from() == Square::A1);
  assert(m.to() == Square::A2);
  assert(m.type() == Move::Type::NORMAL);

  std::cout << "TEST END SUCCESSFULLY .............................."
            << std::endl;
  return 0;
}
