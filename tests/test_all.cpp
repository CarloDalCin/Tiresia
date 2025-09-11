#include <cassert>
#include <iostream>
#include <print>
#include <string>

// Include le tue classi
#include "board.hpp"
#include "castle.hpp"
#include "gamestate.hpp"
#include "libtiresia.hpp"
#include "move.hpp"
#include "piece.hpp"

int main(int argc, char **argv) {
  std::cout << "libtiresia test suite" << std::endl;
  std::cout << "TEST START ........................................"
            << std::endl;

#ifdef DEBUG
  Board b = Board::init_std();
  // Test Board
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
#endif

  GameState gs = GameState::init_std();

  std::string line;
  do {
    gs.print();
    std::printf("> ");
    std::getline(std::cin, line);

    if (line.length() <= 4) {
      std::string from = line.substr(0, 2);
      std::string to = line.substr(2, 2);

      gs.move_piece(Square::from(from), Square::from(to));
    }
    std::cout << line << std::endl;
  } while (line != "quit");

  std::cout << "TEST END SUCCESSFULLY .............................."
            << std::endl;
  return 0;
}
