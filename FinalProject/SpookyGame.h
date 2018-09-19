#ifndef SPOOKYGAME_H
#define SPOOKYGAME_H

#include "ChessGame.h"
#include "Enumerations.h"

#include <string>



class SpookyGame : public ChessGame {

public:

  SpookyGame();

  SpookyGame(const std::string filename, const int game_choice);

  //ADDED(Kyu): performs a move of ghost
  // this need not be defined as virtual in Game.h
  // because Spooky_version of process_turn() will use this function.
  int make_ghost_move();

  int game_over() override; 

  //Question from randy: Kyu why are these virtual? 
  //virtual void run() override;

  int tell_wch_game() override { return 3; }

  void process_turn(Position start, Position end, int board_toggle) override; 
  
protected:

  Position whereGhost;
};


#endif //SPOOKYGAME_H
