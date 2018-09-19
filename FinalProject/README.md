# cs220finalproject
# 601.220 11/11/2017 

TEAM
Justin Huang jhuan104 jhuan104@jhu.edu
Kyu Seok Kim kkim124 kkim124@jhu.edu
Randy Kuang rkuang2 rkuang2@jhu.edu

COMPLETENESS

RUN/COMPILATION
We used the command "make" to compile the code and the command "./play" to run the executable file. 

DESIGN CHOCIES: Differences from original code

Game.h: Added methods

public:
- void print_board(): Makes our code cleaner to have one method that does all the board printing for us in the run() code.
- Player not_player_turn(): Used when we need to find the other player. Specifically helpful for our Prompt printing. We could not just use !player_turn() as this gave a bool type rather than a player type, and we use the player type in prompt printing.
- void save(const std::string filename): Used to save the game

not protected yet but should be moved to protected:
- perform_a_move(Position start, Position end): helper function that moves a piece. This function does not check for any conditions, it is called by make_move() after conditions are checked.

- print_move_prompt(int return_message): function that takes an integer that corresponds to one of the enumerations in enum status in Game.h. This input is then used to print an appropriate prompt. This function is only used to print prompts associated with moving pieces, and thus does not deal with parse errors or save/load errors.

- tell_wch_game(): returns an int 1-3 based on which game is being played

- process_turn(): helped function of run(). This function only calls make_move() and checks game_over() in regular chess and king of the hill chess. However, in spooky chess this function also moves the ghost. 

protected:
- Position w_king / b_king: Position variables that keep track of both white king and black king

- Position causing_check: used by checkmate() to identify the piece causing check and whether it's possible to get out of check

- int _random_gen_count: keeps track of how many times the random generator has been used, only used for SpookyChess

- Note: we also added MOVE_CONQUERED and GHOST_CAPTURE to enum status so that print_move_prompt() could print appropriate prompts in case of those two events

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ChessGame.h: Added methods 
*make_move(Position start, end):
Broke this into several methods. First, valid_move_given_board takes the proposed start and end locations and the player's turn we are checking and sees if the move is valid, excluding anything check-related. After this, we check_for_check to see if the move violates any check-related rules or causes check. If we are still ok to move, we perform_a_move - a method that moves the piece, and updates the king position if we moved it.

*check:
Checks if the current player is under check. To do so, goes through the whole board, and checks if it is a valid_move_given_board for the opponent to move any of his pieces to the king spot. If this is true, we are in check. We also keep track of the location of the piece causing check to use for checking checkmate later.

*checkmate:
We keep track of the locations of the kings and the piece causing check. Uses a four-step process to check for checkmate. First, we only check for checkmate if we are under check. Next, we see if the king can move to any adjacent spot. If not, we check if the piece causing check can be captured. If not, we check if the check trajectory can be blocked. If we cannot move, cannot capture piece and cannot block the check, we are in checkmate.

*stalemate:
The function for checking for stalemate has two steps. First, it makes sure that the opposing player is not currently in check. Then it checks if the opposing player can legally move any of its pieces to any spot on the board. If yes, then the opposing player is not in stalemate and the game continues. If no, then stalemate is declared and the game ends. This solution is far less elegant than what we did for checkmate because there are so many possible ways stalemate can occur.

*check_pawn_conversion():
This function runs at the end of make_move() if make_move() actually moves a piece. The function looks at the piece that was just moved in make_move() and checks if that piece is a pawn that has crossed the board. If so, the pawn is repaced with a queen.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Play.cpp
*run(Game *g):
In the original code given to us, run() was a function under Game.h. We decided to move run() to Play.cpp because collect_game_choice() and determine_new_or_load() were already given to us in Play.cpp and all of these functions deal with UI and actually creating/running through a chess game. Giving run a pointer to a game as a parameter also allows us to not have to rewrite run() because it will automotically choose the correct game_over() and make_move() functions based on what type of chess game was pased in as a parameter.

*we also removed the collect_filename() method that was given to us and incorporated it into determine_new_or_load()

*Note: both collect_game_choice() and determine_new_or_load() will keep printing the prompt until the user gives valid input. It does not give an error message if bad input is given. In run() however, bad input results in an error message being printing as well as a prompt asking for input again.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SpookyGame.h/SpookyGame.cpp/KingGame.h/KingGame.cpp 
*Note 1: SpookyGame and KingGame are classes that inheret from ChessGame
*Note 2: In spooky chess, if the ghost moves to the same spot as it previous was, it will move again 


