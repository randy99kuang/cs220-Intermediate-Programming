#include "SpookyGame.h" //ChessGame.h -> Game.h, ChessPiece.h -> Terminal.h Enu~ Piece~

#include "ChessGame.h"
#include "Prompts.h"
#include "Enumerations.h"
//#include "ChessPiece.h" //Enumerations.h, Piece.h
#include <fstream>
#include <sstream>

#include <string>

using std::istringstream;
using std::ifstream;



SpookyGame::SpookyGame() : ChessGame() {

  //register factory for ghost
  add_factory(new PieceFactory<Ghost>(GHOST_ENUM));

  //make ghost on the board ('a5' position)
  whereGhost.x = 0;
  whereGhost.y = 4;
  init_piece(GHOST_ENUM, NO_ONE, whereGhost);

  srand (322); //random number generator seed
  //note: _random_gen_count is initialized in ChessGame.cpp
  // default constructor: initialized to 0
  // non-default constructor: initialized to the value read from file
}


SpookyGame::SpookyGame(const std::string filename, const int game_choice) : ChessGame(filename, game_choice) {

  //  if (game_choice == SPOOKY_CHESS && wchGame != "spooky") {
  //  Prompts::wrong_game_choice();
    //    std::cout << "hi" << std::endl;
  //  return;
  // }
  
  // update where the ghost was
  for (unsigned int i = 0; i < _pieces.size(); i++) {
    if ( _pieces.at(i) &&
	 _pieces.at(i) -> piece_type() == GHOST_ENUM ) {
      whereGhost.x = i % _width;
      whereGhost.y = i / _height;
    }
  }
  
  //note: seed is updated to loaded-status in ChessGame.cpp
}

int SpookyGame::make_ghost_move() {

  int return_message = 1;  
  // decide where the ghost should go
  unsigned int randNum = rand() % 64; _random_gen_count++;
  // ghost cant attack king or itself(has to move)
  while ( _pieces.at(randNum) &&
	  (_pieces.at(randNum) -> piece_type() == KING_ENUM ||
	   _pieces.at(randNum) -> piece_type() == GHOST_ENUM) ) {
    randNum = rand() % 64; _random_gen_count++;
  }

  // move ghost
  if ( _pieces.at(randNum) ) {
    return_message = GHOST_CAPTURE; 
  }
  
  delete _pieces.at( randNum ); //delete the piece that is going to be killed
  _pieces.at( randNum ) = _pieces.at( index(whereGhost) );
  _pieces.at( index(whereGhost) ) = nullptr;
  whereGhost.x = randNum % _width; //update where ghost is
  whereGhost.y = randNum / _height;

  return return_message; 
}

int SpookyGame::game_over() {
  if (stalemate(player_turn())) {
    return MOVE_STALEMATE;
  }
  if (check(player_turn()) && checkmate(player_turn())) {
    return MOVE_CHECKMATE; 
  }
  if (check(not_player_turn())) {
    return MOVE_OTHER_CHECKMATE; 
  }
  return 0; 
}

void SpookyGame::process_turn(Position start, Position end, int board_toggle) {

  //call all other functions from ChessGame.h,
  //which are part of run functions in ChessGame.cpp
  int prompt = make_move(start, end);
  if (game_over()) {
    if (board_toggle) {
      print_board(); 
    }
    print_move_prompt(game_over());
    return; 
  }

  int ghost_prompt = 0; //default value is 0 because this prints nothing
  //only move the ghost if the piece successfully moved 
  if (prompt > 0) {
    ghost_prompt = make_ghost_move();

    //check if the game is over after the ghost moves 
    if (game_over()) {
      if (board_toggle) {
        print_board(); 
      }
      print_move_prompt(game_over());
      std::cout << "hello" << std::endl; 
      return;
    }
    else if (check(player_turn())) {
      ghost_prompt = MOVE_CHECK;  
    }
  }
  print_move_prompt(prompt);

  //only print the ghost_prompt if ghost actually moved 
  if (prompt > 0) {
    print_move_prompt(ghost_prompt);
  }
}


