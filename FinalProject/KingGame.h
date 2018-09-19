#ifndef KINGGAME_H
#define KINGGAME_H

#include "ChessGame.h"
#include "Enumerations.h"
#include "Prompts.h"
class KingGame : public ChessGame {
public:

 KingGame(): ChessGame() {}
  
  KingGame(const std::string filename, const int game_choice);
  /*
  {
    if (game_choice == KING_OF_THE_HILL && wchGame != "king") {
      Prompts::wrong_game_choice();
      std::cout << "hi" << std::endl;
      return;
    }
  }
  */
  
  int game_over() override;

  int tell_wch_game() override { return 2; }
  
};


#endif 
