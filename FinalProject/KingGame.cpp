#include "KingGame.h"

#include "ChessGame.h"
#include "Prompts.h"
#include "Enumerations.h"

//(RANDY)



KingGame::KingGame(const std::string filename, const int game_choice) : ChessGame(filename, game_choice) {
  //    if (game_choice == KING_OF_THE_HILL && wchGame != "king") {
  //    Prompts::wrong_game_choice();
  //    return;
  //  }
}


int KingGame::game_over() {
  if ((w_king.x == 3 || w_king.x == 4) && (w_king.y == 3 || w_king.y == 4)) {
    return MOVE_CONQUERED; 
  }
  else if ((b_king.x == 3 || b_king.x == 4) && (b_king.y == 3 || b_king.y == 4)) {
    return MOVE_CONQUERED; 
  }
  
  if (stalemate(player_turn())) {
    return MOVE_STALEMATE; 
  }

  if (check(player_turn()) && checkmate(player_turn())) {
    return MOVE_CHECKMATE;
  }
  
  return 0; 
}
