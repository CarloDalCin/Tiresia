#pragma once

#include "bitBoard.hpp"
#include "types.hpp"

// Check if the current pice is pinned to the king
static bool isPinned(const uint64_t& pice_position, pice_t type);

// Retutn the possible moves for a partucular piece
static vector<uint64_t> getMoves(const uint64_t& pice_position, pice_t type);

// Return the possible captures for a particular piece
static vector<uint64_t> getCaptures(const uint64_t& pice_position, pice_t type);

// Check if the current piece can move
static bool canMove(const uint64_t& pice_position, pice_t type);

// Check if the king is in check
static bool isCheck(color_t color);

// Check if the current position is a checkmate
static bool isCheckmate(); // TODO

// Check if the current position is a stalemate
static bool isStalemate(); // TODO

// Check if draw for insufficient material
static bool isDrawForInsufficientMaterial();

// Check if draw for threefold repetition
static bool isDrawForThreefoldRepetition();

// Check if draw for fifty move rule
static bool isDrawForFiftyMoveRule();

// Check if is possible to castle
ststic bool isPossibleToCastle();

// Check if is possible to castle to a particular side
static bool isPossibleToCastleToSide(castle_t side);

// Update the board from a move.from to move.to
// Returns true if the move is legal
// Returns false if the move is illegal
static bool updateBoardFromPosition(piece_t type, move_t<const uint64_t&> move);

// Evaluate the position
static int evaluatePosition();
