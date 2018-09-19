#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath> //abs
#include "Game.h"
#include "ChessGame.h"
#include "Prompts.h"
//#include "Enumerations.h"
//#include "Piece.h" //this is included in Game.h
//                    -> but I need here. I use Player in this file
//               do I still need to include ? and make dependent on Makefile?

#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::istringstream;
using std::ifstream; using std::ofstream;


// Set up the chess board with standard initial pieces
ChessGame::ChessGame(): Game() {
    initialize_factories();
    std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
        KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };
    for (size_t i = 0; i < pieces.size(); ++i) {
        init_piece(PAWN_ENUM, WHITE, Position(i, 1));
        init_piece(pieces[i], WHITE, Position(i, 0));
        init_piece(pieces[i], BLACK, Position(i, 7));
        init_piece(PAWN_ENUM, BLACK, Position(i, 6));
    }
    
    // Tracking King positions 
    w_king.x = 4;
    w_king.y = 0;
    b_king.x = 4;
    b_king.y = 7;

    // Tracking how many times random_generator has been used
    // used only for Spooky Chess.
    _random_gen_count = 0;
}


// Set up the chess board with game state loaded from file
ChessGame::ChessGame(const std::string filename, const int game_choice) : Game() {

    initialize_factories();

    // Add code here to collect piece information from file
    // and call init_piece as needed to create and place each one
    // (see the no-argument constructor for appropriate syntax)

    ifstream fin ( filename );

    if (fin.fail()) {
      Prompts::load_failure();
      return;
    }

    fin >> wchGame; //filename

    //when ChessGame(constructor) selected in loading
    // and other game file is loaded
    if ( (game_choice == STANDARD_CHESS && wchGame != "chess") ||
	 (game_choice == KING_OF_THE_HILL && wchGame != "king") ||
	 (game_choice == SPOOKY_CHESS && wchGame != "spooky") ) {
      Prompts::wrong_game_choice();
      return;
    }

    if (wchGame == "spooky") {

      fin >> _turn; _turn++;
      // _turn in file means how many turns were taken when saved
      // so when loaded, the game should start on the next turn
      fin >> _random_gen_count;
      add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
      srand (322); // update random number generator as last condtion
      for (unsigned int i = 0; i < _random_gen_count; i++) {
        rand();
      }
      
    } else { //chess and king of the hill
      fin >> _turn; _turn++;
    }

    //read each piece
    string line;
    getline(fin, line); //get rid of trailing new_line_character
    while ( getline(fin, line) ) {

      istringstream iss(line);
      
      string wchPosition; int int_buff;
      iss >> int_buff; Player wchPlayer = static_cast<Player>(int_buff);
      iss >> wchPosition;
      iss >> int_buff; PieceEnum wchType = static_cast<PieceEnum>(int_buff);

      unsigned int xpos = wchPosition[0] - 97;
      unsigned int ypos = wchPosition[1] - 49;

      init_piece(wchType, wchPlayer, Position(xpos, ypos));
    }


    //update position of king
    for (unsigned int i = 0; i < _pieces.size(); i++ ) {
      Piece* thePiece = _pieces.at(i);
      if ( thePiece && thePiece -> piece_type() == KING_ENUM) {
	if ( thePiece -> owner() == 0) {
	  w_king.x = i % 8;
	  w_king.y = i / 8;
	}
	if ( thePiece -> owner() == 1) {
	  b_king.x = i % 8;
	  b_king.y = i / 8;
	}	
      }
    }
    fin.close();
}

void ChessGame::save(const std::string filename) {

  ofstream fout (filename);
  
  if (tell_wch_game() == 1) { //standard chess
    fout << "chess" << endl;
    fout << _turn -1 << endl;
    // when saved, _turn value means which 

  } else if (tell_wch_game() == 2) { //king of the hill
    fout << "king" << endl;
    fout << _turn -1 << endl;

  } else { //spooky
    fout << "spooky" << endl;
    fout << _turn -1 << endl;
    fout << _random_gen_count << endl;
  }

  for (unsigned int i = 0; i < _pieces.size(); i++) {
    Piece* thePiece = _pieces.at(i);
    if (thePiece) {
      fout << thePiece -> owner() << " " << (char) (i % 8 + 97) << (i / 8) + 1
           << " " << thePiece ->piece_type() << endl;
    }
  }

  if (fout.fail()) {
    Prompts::save_failure();
  }
  fout.close();
}



// Perform a move, check if we moved the king and update its position
// if we did
void ChessGame::perform_a_move(Position start, Position end) {
  _pieces.at(index(end)) = _pieces.at(index(start));
  _pieces.at(index(start)) = nullptr;
  
  // If we just moved the king, update our king position tracker
  if (_pieces.at(index(end))->piece_type() == 5) {
    // Moved white king
    if (player_turn() == 0) {
      w_king.x = end.x;
      w_king.y = end.y;
    } else {
      b_king.x = end.x;
      b_king.y = end.y;
    }
  }
}

//(RANDY and JUSTIN)
int ChessGame::pawn_conversion(Position end) {
  //QUESTION!!!: someone else wrote this question, but why does this work without #include-ing Piece.h????
  // We include chessgame.h which includes chesspiece.h which includes piece.h (i think this is why)
  Piece *temp = get_piece(end);
  int owner = temp->owner();
  int type = temp->piece_type();

  // If the piece that just moved is a pawn
  if (type == 0) {
    // White pawn conversion
    if (owner == 0 && end.y == 7) {
      _pieces.at(index(end)) = nullptr;
      delete temp;
      init_piece(QUEEN_ENUM, WHITE, end);
    }
    // Black pawn conversion
    else if (owner == 1 && end.y == 0) {
      _pieces.at(index(end)) = nullptr;
      delete temp;
      init_piece(QUEEN_ENUM, BLACK, end); 
    }
    return 1;
  }
  return 0;
}

void ChessGame::process_turn(Position start, Position end, int board_toggle) {
  int prompt = make_move(start, end);
  if (game_over()) {
    if (board_toggle) {
      print_board(); 
    }
    print_move_prompt(game_over());
    return; 
  }
  print_move_prompt(prompt); 
}

// Perform a move from the start Position to the end Position
// The method returns an integer with the status
// >= 0 is SUCCESS, < 0 is failure
int ChessGame::make_move(Position start, Position end) {
  int move_type = valid_move_given_board(start, end, player_turn());
  Piece *temp = get_piece(end);
  // If move is valid, beyond exposing king to check
  if (move_type > 0) {
    move_type = check_for_check(start, end, move_type);
    if (move_type > 0) {
      perform_a_move(start, end);
      delete temp;
      _turn++;
      // If the new queen from the pawn conversion causes check
      if (pawn_conversion(end) && check(player_turn())) {
	move_type = MOVE_CHECK;
      }
    }
      // All of this is only for spooky chess
      /*if (move_type > 0) {
	// Ghost moves here
	// After ghost moves, need to check for check
	}*/
  }
  return move_type;
}

int ChessGame::checkmate(Player pturn) {
  //cout << valid_adjacent_king_move() << endl;
  //cout << causing_check_piece_capturable() << endl;
  if (!valid_adjacent_king_move(pturn) && !causing_check_piece_capturable(pturn) && !blockable_check(pturn)) {
    return 1;
  }
  return 0;
}

int ChessGame::blockable_check(Player pturn) {
  // Calling check here to reset the causing_check position
  // Can sometimes be the wrong position after valid_adjacent_king_move()
  // and causing_check_piece_capturable because we try moving the king around
  // and checking for check under the moved-king conditions. This sometimes 
  // results in the wrong piece being tracked as causing_check. Calling
  // check like this with the king in its normal position resets this.
  check(player_turn());
  // Fill in theoretical trajectory of piece causing check to the king
  vector<Position> trajectory;
  if (pturn == WHITE) {
    get_piece(causing_check)->valid_move_shape(causing_check, w_king, trajectory);
  } else {
    get_piece(causing_check)->valid_move_shape(causing_check, b_king, trajectory);
  }
  // See if the current player has any pieces that can move in any spots between the piece causing check and the king
  for (unsigned i = 0; i < _pieces.size(); i++) {
    if (_pieces.at(i) != nullptr && _pieces.at(i)->owner() == pturn) {
      Position start;
      start.x = i % _width;
      start.y = i / _width;
      //      cout
      // For each piece, see if it can move to any of the trajectory spots, excluding the start and end (start is piece causing check - this case would be caught by causing_check_piece_capturable. End is our own king)
      for (unsigned j = 0; j < trajectory.size() - 1; j++) {
	int val = valid_move_given_board(start, trajectory.at(j), pturn);  
	if (val >= 0 && check_for_check(start, trajectory.at(j), pturn) >= 0) {
	  return 1;
	}
      }
    }
  }
  return 0;
}

int ChessGame::causing_check_piece_capturable(Player pturn) {
  for (unsigned i = 0; i < _pieces.size(); i++) {
    if (_pieces.at(i) != nullptr && _pieces.at(i)->owner() == pturn) {
      Position start;
      start.x = i % _width;
      start.y = i / _width; 
      int val = valid_move_given_board(start, causing_check, pturn);
      if (val >= 0 && check_for_check(start, causing_check, pturn) >= 0) {
	return 1;
      }
    }
  }
  return 0;
}

// Checks to see if the king can move to any adjacent spot
// If so, this is not checkmate
int ChessGame::valid_adjacent_king_move(Player pturn) {
  Position adjacent;
  int val;
  if (pturn == WHITE) {
    for (int i = -1; i <= 1; i++) {
      adjacent.x = w_king.x + i;
      for (int j = -1; j <= 1; j++) {
	adjacent.y = w_king.y + j;
	val = valid_move_given_board(w_king, adjacent, WHITE);
	if (val >= 0 && check_for_check(w_king, adjacent, val) >= 0) {
	  return 1;
	}
      }
    }
  } else {
    for (int i = -1; i <= 1; i++) {
      adjacent.x = b_king.x + i;
      for (int j = -1; j <= 1; j++) {
	adjacent.y = b_king.y + j;
	val = valid_move_given_board(b_king, adjacent, BLACK);
	if (val >= 0 && check_for_check(b_king, adjacent, val) >= 0) {
	  return 1;
	}
      }
    }
  }
  // No moves were possible for the king itself
  return 0;
}

int ChessGame::check_for_check(Position start, Position end, int move_type) {
  Piece *temp = get_piece(end);
  // If we start this move in a checked state
  if (check(player_turn())) {
    perform_a_move(start, end);
    // If we are still in check after attempting a move
    if (check(player_turn())) {
      perform_a_move(end, start);
      _pieces.at(index(end)) = temp;
      return MOVE_ERROR_MUST_HANDLE_CHECK;
    } else {
      // We handled the check, valid move
      //	delete temp;
      _turn++;
      if(check(player_turn())) {
	_turn--;
	perform_a_move(end, start);
	_pieces.at(index(end)) = temp;
	return MOVE_CHECK;
      }
    }
  } else {
    perform_a_move(start, end);
    // If move exposes self check
    if(check(player_turn())) {
      perform_a_move(end, start);
      _pieces.at(index(end)) = temp;
      return  MOVE_ERROR_CANT_EXPOSE_CHECK;
    } else {
      // Fully valid move
      //	delete temp;
      _turn++;
      // If move checks opponent
      if(check(player_turn())) {
	_turn--;
	perform_a_move(end, start);
	_pieces.at(index(end)) = temp;
	return MOVE_CHECK;
      }
    }
  }

  _turn--;
  perform_a_move(end, start);
  _pieces.at(index(end)) = temp;
  return move_type;
}

void ChessGame::print_move_prompt(int return_message) {
  switch(return_message) {
  case -7:
    Prompts::out_of_bounds(); break;
  case -6:
    Prompts::no_piece(); break;
  case -5:
    Prompts::blocked(); break;
  case -4:
    Prompts::cannot_castle(); break;
  case -3:
    Prompts::must_handle_check(); break;
  case -2:
    Prompts::cannot_expose_check(); break;
  case -1:
    Prompts::illegal_move(); break;
  case 1: // normal successful move, no message
    break;
  case 2:
    Prompts::check(not_player_turn()); break;
  case 3:
    Prompts::capture(not_player_turn()); break;
  case 4:
    Prompts::checkmate(not_player_turn());
    Prompts::win(not_player_turn(), turn() - 1);
    Prompts::game_over();
    break;
  case 5:
    Prompts::checkmate(player_turn());
    Prompts::win(player_turn(), turn() - 1);
    Prompts::game_over();
    break;
  case 6:
    Prompts::stalemate();
    Prompts::game_over();
    break;
  case 7:
    Prompts::conquered(not_player_turn());
    Prompts::win(not_player_turn(), turn() - 1);
    Prompts::game_over();
    break;
  case 8:
    Prompts::ghost_capture(); break;
  case 9:
    Prompts::win(not_player_turn(), turn() - 1); break;
  case 10:
    Prompts::game_over(); break;
  default:
    cout << "Error: Should not get here";
  }
}

// Method to check if a move involving move_check's piece from start to end is valid
// Added the Player parameter to check for "check" before actually changing the turn (thus, we could not solely rely on player_turn())
int ChessGame::valid_move_given_board(Position start, Position end, Player move_check) {

  if ( get_piece( start ) == nullptr) {
    //    Prompts::no_piece();
    return MOVE_ERROR_NO_PIECE;
  }
  
  if ( get_piece( start ) -> owner() != move_check ) {
    //    Prompts::no_piece();
    return MOVE_ERROR_NO_PIECE;
  }
  
  //check if start, end are the same
  if ( (start.x == end.x) && (start.y == end.y) ) {
    //    Prompts::illegal_move();
    return MOVE_ERROR_ILLEGAL;
  }
  //check if end_position is in_bound
  if ( ! valid_position(end) ) {
    //    Prompts::out_of_bounds();
    return MOVE_ERROR_OUT_OF_BOUNDS;
  }


  
  //####################START#######################//
  //################valid_move_shape################//
  //####################START#######################//

  // 1. check valid_move_shape.
  //    populates trajectory 
  vector<Position> trajectory;
  if (get_piece(start) ->
      valid_move_shape(start, end, trajectory) < 0 ) {
    //    Prompts::illegal_move();
    return MOVE_ERROR_ILLEGAL;
  }
  
  // 2. additional check for PAWN
  if ( get_piece(start)->piece_type() == PAWN_ENUM ) {
    int xdiff = end.x - start.x;
    // 2-1. non-attack move: cannot be blocked
    if (xdiff == 0) {
      for (unsigned int i = 1; i < trajectory.size(); i++) {
	if ( get_piece( trajectory.at(i) ) ) {
	  //	  Prompts::blocked();
	  return MOVE_ERROR_BLOCKED;
	}
      }
    }
    // 2-2. attack-move
    if ( abs(xdiff) == 1 ) {
      Piece* start_piece = get_piece( start );
      Piece* end_piece = get_piece( trajectory.back() );

      //attack-move to empty space
      if ( ! end_piece ) {
	//	Prompts::illegal_move();
	return MOVE_ERROR_ILLEGAL;	
      }
      //attack opponent
      //this is only true in white(0) and black(0) combination
      if ( start_piece -> owner() + end_piece -> owner()
	   == 1) {
	//	Prompts::capture( player_turn() );
	return MOVE_CAPTURE;
	
	//blocked by own_piece or ghost
      } else { 
	//	Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }
  } //additional check for PAWN end.
  
  //#####################END########################//
  //################valid_move_shape################//
  //#####################END########################//

  

  //check if blocked (using trajectory)
  //except start position, end position
  for (unsigned int i=1; i < trajectory.size() - 1; i++) {
    if ( get_piece( trajectory.at(i) ) ) {
      //true when occupied (ptr returned)
      //false when empty (nullptr returned)
      //      Prompts::blocked();
      return MOVE_ERROR_BLOCKED;
    }
  }

  
  
  //NOTE for RANDY and JUSTIN
  //######
  //#NOTE#: other checks are necessary here
  //
  //###### ex) checkmate, stalemate, ....
  // 2 cases: causing check, or in-check.

  
  //check end_position
  //check if end_position == empty
  //and perform a move
  if ( get_piece( end ) == nullptr) {
    return SUCCESS;

    //check if end_position == own_piece
  } else if (get_piece( end ) -> owner() ==
             get_piece( start)-> owner() ) {
    //    Prompts::blocked();
    return MOVE_ERROR_BLOCKED;
   
    //check if end_position == opponent_piece (white 0, black 1)
    //capture move
  } else if (get_piece( end ) -> owner() +
	     get_piece( start ) -> owner()
	     == 1 ) {
    
    //    Prompts::capture( player_turn() );
    return MOVE_CAPTURE;
    
  } else { //ghost
    //    Prompts::blocked();
    return MOVE_ERROR_BLOCKED;
  }
}

// Report whether the chess game is over
int ChessGame::game_over() {
  //QUESTION!!!: I don't see a point in returning move_stalemate or move_checkmate because if there's a stalemate/checkmate then the game ends anyway?
  if (stalemate(player_turn())) {
    //    Prompts::stalemate();
    return MOVE_STALEMATE;
  }
  // Will only check checkmate if we are in check
  if (check(player_turn()) && checkmate(player_turn())) {
    return MOVE_CHECKMATE;
  }

  // else if (checkmate()) { return MOVE_CHECKMATE; }
  return 0;

}


void ChessGame::run() {

  //now the game actually starts
  //string input;
  int board_toggle = 0;
  while (!(game_over())) {
    if (board_toggle) {
      print_board();
    }
     
    Prompts::player_prompt(player_turn(), turn());
    string input;
    getline(cin, input);
    //cout << input << endl; 

    if (input == "board") {
      if(board_toggle == 0) {
	board_toggle = 1;
	print_board();
      }
      else {
	board_toggle = 0;
      }
      Prompts::player_prompt(player_turn(), turn());
      getline(cin, input);
    }

    if (input == "q") {
      //do we need extra thing here to prevent leaks? or does the destructor take care of everything
      break;
    }
    else if (input == "save") {
      //(Kyu)
      Prompts::save_game();
      string filename;
      getline(cin, filename);
      //      cin >> filename;
      //      cin.get();
      save(filename);
      
      //more stuff needed here once save/load functions are created
      // (Kyu to Randy): load function should not be considered in this loop
    }
    else if (input == "forfeit") {
      Prompts::win(not_player_turn(), turn());
      Prompts::game_over();
      break;
    }
    else if (isalpha(input[0]) && isalpha(input[3]) && isdigit(input[1]) && isdigit(input[4]) && isspace(input[2])) {
      cout << "move processed, test message" << endl;
      Position start, end;

      // Converting from ascii to board coordinates
      start.y = input[1] - 49;
      end.y = input[4] - 49; 
      if (islower(input[0]) && islower(input[3])) {
        start.x = input[0] - 97;
        end.x = input[3] - 97;
      }
      else {
	start.x = input[0] - 65;
	end.x = input[3] - 65;
      }

      // Handles all possible move cases
      //this part can be moved back into ChessGame.cpp/SpookyGame/KingGame if necessary
      /*
      int prompt = make_move(start, end);
      if (game_over()) {
	if(board_toggle) {
	  print_board();
	}
	
	print_move_prompt(game_over()); 
       	break;
      }
      print_move_prompt(prompt);
      */
      process_turn(start, end, board_toggle); 
    }
    else {
      Prompts::parse_error();
    }

  }
}


/*
//(Randy)
void ChessGame::run() {
  //now the game actually starts  
  //string input; 
  int board_toggle = 0; 
  while (!game_over()) {
    if (board_toggle) {
      print_board();
    }

    Prompts::player_prompt(player_turn(), _turn);
    string input; 
    getline(cin, input);
    cout << input << endl; 

    if (input == "board") {
      if(board_toggle == 0) {
	board_toggle = 1;
	print_board();
      }
      else {
	board_toggle = 0;
      }
      Prompts::player_prompt(player_turn(), _turn); 
      getline(cin, input); 
    }

    
    if (input == "q") {
      //do we need extra thing here to prevent leaks? or does the destructor take care of everything 
      break;
    }
    else if (input == "save") {
      cout << "save selected, test message" << endl;
      //more stuff needed here once save/load functions are created 
    }
    else if (input == "forfeit") {
      _turn++; // Making the win prompt state the other player as the winner
      Prompts::win(player_turn(), _turn - 1);
      Prompts::game_over();
      break; 
    }
    else if (islower(input[0]) && islower(input[3]) && isdigit(input[1]) && isdigit(input[4]) && isspace(input[2])) { //this feels very tacky
      // Justin responding to randy's comment: It looks like it works well though
      cout << "move processed, test message" << endl; 
      Position start, end;
      
      // Converting from ascii to board coordinates
      start.x = input[0] - 97;
      start.y = input[1] - 49;
      end.x = input[3] - 97;
      end.y = input[4] - 49;

      // Handles all possible move cases
      make_move(start, end);
      if (game_over()) {
	Prompts::game_over();
	break; 
      }


    }
    else {
      Prompts::parse_error();
    }

  }
}
*/

// Check for check by passing a King's position and seeing
// if it is a valid move for an opponent to move any piece to
// the king's spot
// TO CHANGE: RETURN THE POSITION OF THE PIECE THAT CAUSES CHECK
// IF NO CHECK, RETURN POS x = -1 y = -1

int ChessGame::check(int pturnval) {
  for (unsigned i = 0; i < _pieces.size(); i++) {
    if (_pieces.at(i) != nullptr && _pieces.at(i)->owner() == static_cast<Player>(!pturnval)) {
      // Getting the position of the attacking piece from its index
      Position attack_pos;
      attack_pos.x = i % _width;
      attack_pos.y = i / _width;
      // If the piece can attack the king
      if (pturnval == 0) {
	// If black can attack white's king
	if (valid_move_given_board(attack_pos, w_king, BLACK) > 0) {
	  // Keeping track of location of piece causing check
	  // Used when we check for checkmate - need to see if we can
	  // capture this position
	  causing_check.x = attack_pos.x;
	  causing_check.y = attack_pos.y;
	  return 1;
	}
      } else {
	// If white can attack black's king
	if (valid_move_given_board(attack_pos, b_king, WHITE) > 0) {
	  // Keeping track of location of piece causing check
	  // Used when we check for checkmate - need to see if we can
	  // capture this position
	  causing_check.x = attack_pos.x;
	  causing_check.y = attack_pos.y;
	  return 1;
	}
      }
    }
  }
  return 0;
}

//(RANDY)
//Strategy: Check every one of your pieces and try to move them to every possible location. If any piece can move, then you're not in stalemate
int ChessGame::stalemate(Player pturn) {
  //if the board is currently under check, there is no way stalemate is possible
  //NOTE!!!!: if we check stalemate after checking for check/checkmate, this part isn't necessary
  if (check(pturn)) {
    return 0; 
  }
  
  for (unsigned i = 0; i < _pieces.size(); i++) {
    for (unsigned board_position = 0; board_position < _pieces.size(); board_position++) {
      if (_pieces.at(i) != nullptr && _pieces.at(i)->owner() == pturn) {
	Position start, end;
	start.x = i % _width;
	start.y = i / _width; 
	end.x = board_position % _width;
	end.y = board_position / _width;
	int val = valid_move_given_board(start, end, get_piece(start)->owner());
	if (val >= 0 && check_for_check(start, end, get_piece(start)->owner()) >= 0) {
	  return 0;
	}
      }
    }
  }
  cout << "stalemate is here" << endl;
  //If this point of the stalemate() method is reached, it means that no move was possible, signifying that the board is in stalemate. 
  return 1; 
}
    
// Prepare the game to create pieces to put on the board
void ChessGame::initialize_factories() {

    // Add all factories needed to create Piece subclasses
    add_factory(new PieceFactory<Pawn>(PAWN_ENUM));
    add_factory(new PieceFactory<Rook>(ROOK_ENUM));
    add_factory(new PieceFactory<Knight>(KNIGHT_ENUM));
    add_factory(new PieceFactory<Bishop>(BISHOP_ENUM));
    add_factory(new PieceFactory<Queen>(QUEEN_ENUM));
    add_factory(new PieceFactory<King>(KING_ENUM));
}
