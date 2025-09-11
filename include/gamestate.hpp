#pragma once

#include "board.hpp"
#include "castle.hpp"
#include "move.hpp"
#include "piece.hpp"

class GameState {
private:
  Board board; // 176 bytes

  // half move = move for 1 player
  // // count of moves without a capture or pawn move (in
  // rare case the limit increase up to 150 half move)
  uint16_t _halfMoveClock; // 2 bytes
  // full move = move for both players
  uint16_t _fullMoveNumber;   // 2 bytes
  CastleRights _castleRights; // 1 byte
  Square _enPassantSquare;    // 1 byte
  Piece::Color _turn;         // 1 byte

public:
  // Default constructor with a standard position
  inline GameState() {
    GameState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  }
  // Constructors from FEN
  inline GameState(const std::string &fen) {
    auto const match = fen_validator(fen);
    if (!match)
      throw std::runtime_error("Invalid FEN");

    board = Board((*match)[0].str());
    _turn = (*match)[1] == "w" ? Piece::Color::WHITE : Piece::Color::BLACK;
    _castleRights = CastleRights::from((*match)[2].str());
    _enPassantSquare =
        (*match)[3] == "-"
            ? Square::NONE                     // no en passant
            : Square::from((*match)[3].str()); // en passant position
    _halfMoveClock = std::stoi((*match)[4].str());
    _fullMoveNumber = std::stoi((*match)[5].str());
  }

  static inline GameState init_std() { return GameState(); }
  static inline GameState init_960() {
    return GameState(
        "nqrkrbbn/pppppppp/8/8/8/8/PPPPPPPP/NQRKRBBN w KQkq - 0 1");
  }

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

public:
  // Getters
  constexpr uint16_t halfMoveClock() const { return _halfMoveClock; }
  constexpr uint16_t fullMoveNumber() const { return _fullMoveNumber; }
  constexpr CastleRights castleRights() const { return _castleRights; }
  constexpr Square enPassantSquare() const { return _enPassantSquare; }
  constexpr Piece::Color turn() const { return _turn; }

  inline void print() { board.print(Board::get_utf8_piece); }
  constexpr void remove_piece(Square sq) { board.remove_piece(sq); }
  constexpr void move_piece(const Move &move) { move_piece(move); }
  constexpr void move_piece(Square from, Square to) {
    board.move_piece(from, to);
  }
  constexpr void set_piece(Square to, Piece p = Piece::empty()) {
    board.set_piece(to, p);
  }
};
