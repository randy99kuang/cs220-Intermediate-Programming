#include <iostream>
#include <cassert>
#include <cctype>
#include <string>

#include "Game.h"
#include "Prompts.h"
#include "Piece.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

Game::~Game() {
  
  // Delete the factories used to generate pieces
  for (size_t i = 0; i < _registered_factories.size(); i++) {
    delete _registered_factories[i];
  }
  
  // Delete any other dynamically-allocated resources here
  // Delete all the pieces on board
  for (unsigned int i = 0; i < _pieces.size(); i++ ) {
    if ( _pieces.at(i) ) {
      delete _pieces.at(i);
    }
  }
  
}

void Game::print_board() const{
  Terminal::color_fg(true, Terminal::DEFAULT_COLOR);
  // Printing characters on top of board
  cout << "  ";
  for (unsigned i = 0; i < _width; i++) {
    cout << "  " << (char) ('A' + i) << "   ";
  }
  cout << endl;

  // Printing board with numbers on both sides
  //  cout << "  ---------------------------------------------------------" << endl
  for (unsigned i = 0; i < _height; i++) {
    Terminal::color_bg(Terminal::DEFAULT_COLOR);
    cout << "  ";
    for (unsigned j = 0; j < _width; j++) {
      if ((j + i) % 2 == 0) {
	Terminal::color_bg(Terminal::BLACK);
      } else {
	Terminal::color_bg(Terminal::WHITE);
      }
      cout << "      ";
    }
    Terminal::color_bg(Terminal::DEFAULT_COLOR);
    cout << endl;
    cout << 8 - i << " ";

    for (unsigned j = 0; j < _width; j++) {
      unsigned ChessR = (_height - 1) - i; //index of row in chess-board representation (Kyu)
      unsigned wchPiece = index( Position(j, ChessR) );
      Piece* thePiece = _pieces[ wchPiece ];
      if ((j + i) % 2 == 0) {
	Terminal::color_bg(Terminal::BLACK);
      } else {
	Terminal::color_bg(Terminal::WHITE);
      }

      if (thePiece != nullptr) {
	//      Terminal::color_fg(true, Terminal::RED);
	cout << "  ";
	if ((j + 1) % 2 == 0) {
	  print_icon(thePiece -> piece_type(), thePiece -> owner(), 1);
	} else {
	  print_icon(thePiece -> piece_type(), thePiece -> owner(), 0);
	}
	cout << "   ";
	Terminal::color_fg(true, Terminal::DEFAULT_COLOR);
      } else {
	cout << "      ";
      }
    }
    Terminal::color_bg(Terminal::DEFAULT_COLOR);
    cout << " " << 8 - i;
    cout << endl;

    cout << "  ";
    for (unsigned j = 0; j < _width; j++) {
      if ((j + i) % 2 == 0) {
	Terminal::color_bg(Terminal::BLACK);
      } else {
	Terminal::color_bg(Terminal::WHITE);
      }
      cout << "      ";
    }
    Terminal::color_bg(Terminal::DEFAULT_COLOR);
    cout << endl;
  }

  // Printing characters on bottom of board
  Terminal::color_bg(Terminal::DEFAULT_COLOR);
  cout << "  ";
  for (unsigned i = 0; i < _width; i++) {
    cout << "  " << (char) ('A' + i) << "   ";
  }
  cout << endl;
  Terminal::color_fg(true, Terminal::DEFAULT_COLOR);
}

				  

/*void Game::print_board() const{
  // Printing characters on top of board

  ///Commented out by (Kyu) : this is just printingat the bottom left piece's owner
  //cout << _pieces[1] -> owner();
  cout << "  ";
  for (unsigned i = 0; i < _width; i++) {
    cout << "   " << (char) ('A' + i) << "   ";
  }
  cout << endl;

  // Printing board with numbers on both sides
  cout << "  ---------------------------------------------------------" << endl;
  for (unsigned i = 0; i < _height; i++) {
    cout << "  |";
    for (unsigned j = 0; j < _width; j++) {
      cout << "      |";
    }
    
    cout << endl;
    cout << 8 - i << " |";
    
    for (unsigned j = 0; j < _width; j++) {
      unsigned ChessR = (_height - 1) - i; //index of row in chess-board representation (Kyu)
      unsigned wchPiece = index( Position(j, ChessR) );
      Piece* thePiece = _pieces[ wchPiece ];
      
      if (thePiece != nullptr) {
	cout << "  " << gen_icon( thePiece -> piece_type(), thePiece -> owner())  << "   |";
      } else {
	cout << "      |";
      }
    }
    cout << " " << 8 - i;
    cout << endl;
    
    cout << "  |";
    for (unsigned j = 0; j < _width; j++) {
      cout << "      |";
    }
    cout << endl;
    cout << "  ---------------------------------------------------------" << endl;
  }

  // Printing characters on bottom of board
  cout << "  ";
    for (unsigned i = 0; i < _width; i++) {
    cout << "   " << (char) ('A' + i) << "   ";
  }
  cout << endl;
  }*/



// Create a Piece on the board using the appropriate factory.
// Returns true if the piece was successfully placed on the board.
bool Game::init_piece(int piece_type, Player owner, Position pos) {

    Piece* piece = new_piece(piece_type, owner);
    if (!piece) return false;

    // Fail if the position is out of bounds
    if (!valid_position(pos)) {
        Prompts::out_of_bounds();
        return false;
    }

    // Fail if the position is occupied
    if (get_piece(pos)) {
        Prompts::blocked();
        return false;
    }
    _pieces[index(pos)] = piece;
    return true;
}

// Get the Piece at a specified Position.  Returns nullptr if no
// Piece at that Position or if Position is out of bounds.
Piece* Game::get_piece(Position pos) const {
    if (valid_position(pos))
        return _pieces[index(pos)];
    else {
        Prompts::out_of_bounds();
        return nullptr;
    }
}


// Search the factories to find a factory that can translate
//`piece_type' into a Piece, and use it to create the Piece.
// Returns nullptr if factory not found.
Piece* Game::new_piece(int piece_type, Player owner) {

    PieceGenMap::iterator it = _registered_factories.find(piece_type);
    
    if (it == _registered_factories.end()) { // not found
        std::cout << "Piece type " << piece_type << " has no generator\n";
        return nullptr;
    } else {
        return it->second->new_piece(owner);
    }
}



// Add a factory to the Board to enable producing
// a certain type of piece. Returns whether factory
// was successfully added or not.
bool Game::add_factory(AbstractPieceFactory* piece_gen) {
    // Temporary piece to get the ID
    Piece* p = piece_gen->new_piece(WHITE);
    int piece_type = p->piece_type();
    delete p;

    PieceGenMap::iterator it = _registered_factories.find(piece_type);
    if (it == _registered_factories.end()) { // not found, so add it
        _registered_factories[piece_type] = piece_gen;
        return true;
    } else {
        std::cout << "Piece type " << piece_type << " already has a generator\n";
        return false;

    }
}

void Game::print_icon(const int piece_type, const int player, const int origBgCol) const{
  // Black
  if (player == BLACK) {
    Terminal::color_fg(true, Terminal::RED);
    switch(piece_type) {
    case 0: // Pawn
      cout << "\u265f";
      return;
    case 1: // Rook
      cout << "\u265c";
      return;
    case 2: // Knight
      cout << "\u265e";
      return;
    case 3: // Bishop
      cout << "\u265d";
      return;
    case 4: // Queen
      cout << "\u265b";
      return;
    case 5: // King
      cout << "\u265a";
      return;
    }
  }
  // White
  else if (player == WHITE) {
    Terminal::color_fg(true, Terminal::BLUE);
    switch(piece_type) {
    case 0: // Pawn
      cout << "\u2659";
      return;
    case 1: // Rook
      cout << "\u2656";
      return;
    case 2: // Knight
      cout << "\u2658";
      return;
    case 3: // Bishop
      cout << "\u2657";
      return;
    case 4: // Queen
      cout << "\u2655";
      return;
    case 5: // King
      cout << "\u2654";
      return;
    }
  }

  //Kyu: Ghost doesn't belong to anyone.
  Terminal::color_fg(false, Terminal::WHITE);
  Terminal::color_bg(Terminal::GREEN);
  cout << "G";
  if (origBgCol == 0) {
    Terminal::color_bg(Terminal::WHITE);
  } else {
    Terminal::color_bg(Terminal::BLACK);
  }
}
