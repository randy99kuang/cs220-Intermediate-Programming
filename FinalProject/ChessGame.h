#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Game.h"
#include "ChessPiece.h"

#include <string>

class ChessGame : public Game {

public:

    // Creates new game in standard start-of-game state 
    ChessGame();

    // Creates game with state indicated in specified file
    ChessGame(const std::string filename, const int game_choice);

    // Performs a move, updates king position if the king is moved
    virtual void perform_a_move(Position start, Position end) override;

    // Checks for valid move from start to end, given all board conditions
    virtual int valid_move_given_board(Position start, Position end, Player move_check);

    
    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    virtual int make_move(Position start, Position end) override;

    virtual void process_turn(Position start, Position end, int board_toggle) override; 
    
    // Reports whether the chess game is over
    //(RANDY) i made game_over() not const because game_over() uses stalemate() which is not const because it has to test move pieces around 
    virtual int game_over() override;

    // (Kyu)saves the game
    // returns 1 on success, and 0 on failure
    virtual void save(const std::string filename) override;

    // (Kyu)
    virtual int tell_wch_game() override { return 1; } 
    
    virtual void run() override;

protected:

    // Create all needed factories for the kinds of pieces
    // used in chess (doesn't make the actual pieces)
    virtual void initialize_factories();

    // Checks if pturnval player is in check
    int check(int pturnval);

    // Checking
    int check_for_check(Position start, Position end, int move_type);
    
    //QUESTION!!!: do stalemate() and check() need to be virtual?
    int stalemate(Player pturn); 

    // Checks if pturn player is under checkmate
    int checkmate(Player pturn);

    // Checks if the pturn player's king can move anywhere
    int valid_adjacent_king_move(Player pturn);

    // Checks if the piece causing check to the current player can
    // be captured by pturn player
    int causing_check_piece_capturable(Player pturn);

    // Checks if the check can be blocked by pturn player
    int blockable_check(Player pturn);
    
    // Prints the correct move result prompt based on the return value of the move.
    void print_move_prompt(int return_message);

    // Checks if a pawn has moved to the end of the board. If so, converts pawn to queen and returns 1. If not, returns 0
    int pawn_conversion(Position end);

    //this is used when game is loaded
    //checking if appropriate savefile is loaded for each game
    // ex. chess file for chess-load. king file for king-load
    string wchGame;
};

#endif // CHESS_GAME_H
