#include "ChessPiece.h"
#include "Game.h"
#include <cmath>
#include <vector>
#include <algorithm>

using std::vector;
using std::max;

// valid_move_shape
// Returns an integer representing move shape validity
// where a value >= 0 means valid, < 0 means invalid.
// also populates a vector of Positions with the trajectory
// followed by the Piece from start to end



int King::valid_move_shape(Position start, Position end, vector<Position>& trajectory) const {

  int xdiff = end.x - start.x;
  int ydiff = end.y - start.y;

  if ( abs(xdiff) > 1 || abs(ydiff) > 1 ) {
    return MOVE_ERROR_ILLEGAL;

  } else { //valid move
    trajectory.push_back( start  );
    trajectory.push_back( end );
    return SUCCESS;
  }
  
}

int Knight::valid_move_shape(Position start, Position end, vector<Position>& trajectory) const {


  int xdiff = end.x - start.x;
  int ydiff = end.y - start.y;

  if ( ( abs(xdiff)==2 && abs(ydiff)==1 ) ||
       ( abs(xdiff)==1 && abs(ydiff)==2 ) ) {
    trajectory.push_back( start );
    trajectory.push_back( end );
    return SUCCESS;
    
  } else {
    return MOVE_ERROR_ILLEGAL;
  } 
}


// For PAWN, functionality of valid_move_shape is same
// as other valid_move_shape.
// But this does check if in initial position
// and populates 2-forward move only when in initial position

int Pawn::valid_move_shape(Position start, Position end, vector<Position>& trajectory) const {

  int xdiff = end.x - start.x;
  int ydiff = end.y - start.y;

  int ydirection = 1; //White
  if (_owner) {
    ydirection = -1; //Black
  }

  //moving one up(white) or down(black)
  //forward move || attack move
  if ( ( (start.y + ydirection) == end.y ) &&
       ( ( start.x == end.x ) || abs(xdiff) == 1 ) ) {
    trajectory.push_back(start);
    trajectory.push_back(end);
    return SUCCESS;
  }

  //moving two up(white) or down(black)
  //start y-position is 1(white), 6(black)
  if (start.y == 1 + (unsigned int) (_owner)*5 ) {
    if ( start.y + ydirection * 2 == end.y &&
	 xdiff == 0 ) {
      for (int i = 0; i <= abs(ydiff); i++) {
	trajectory.push_back( Position(start.x, start.y + i * ydirection) );
      }
      return SUCCESS;
    }
  }

  return MOVE_ERROR_ILLEGAL;
}



//temp comment: Positions contains an x coordinate and a y coordinate that start at the bottom left hand corner of the chess board and start at (0, 0)
int Rook::valid_move_shape(Position start, Position end, vector<Position>& trajectory) const {
  int xdiff = end.x - start.x;
  int ydiff = end.y - start.y;
  int xdirection, ydirection; 
  if (start.x != end.x && start.y != end.y)
    return MOVE_ERROR_ILLEGAL;
  //this two else if loops fill in the vector using and first set xdirection and ydirection to either -1 or 1 depending on whether the Rook is moving up, down, left, or right
  else {
    if (xdiff != 0) {
      xdirection = xdiff / abs(xdiff);
      ydirection = 0;
    } else if (ydiff != 0) {
      ydirection = ydiff / abs(ydiff);
      xdirection = 0;
    }
    for (int i = 0; i <= max(abs(xdiff), abs(ydiff)); i++) {
      trajectory.push_back(Position(start.x + i * xdirection, start.y + i * ydirection));
    }
  }
  return SUCCESS;   
}


int Bishop::valid_move_shape(Position start, Position end, vector<Position>& trajectory) const {
  int xdiff = end.x - start.x;
  int ydiff = end.y - start.y;
  int xdirection, ydirection; 
  if (abs(xdiff) != abs(ydiff))
    return MOVE_ERROR_ILLEGAL;
  //this else loop is for filling in the vector, basically it first finds whether the end position is to the left or right of start using xdirection then finds whether its higher or lower using ydirection
  else if (abs(xdiff) == abs(ydiff)) {
    xdirection = xdiff / abs(xdiff);
    ydirection = ydiff / abs(ydiff);
    for (int i = 0; i <= abs(xdiff); i++) {
      trajectory.push_back(Position(start.x + i * xdirection, start.y + i * ydirection));
    }
  }

  return SUCCESS;
}


// POTENTIALLY TRY TO MAKE THIS A COMBINATION OF BISHOP AND ROOK VALID_MOVE_SHAPE
int Queen::valid_move_shape(Position start, Position end, vector<Position>& trajectory) const {
  int xdiff = end.x - start.x;
  int ydiff = end.y - start.y;
  int xdirection, ydirection;
  
  if ((start.x != end.x && start.y != end.y) && (abs(xdiff) != abs(ydiff)))
    return MOVE_ERROR_ILLEGAL;
  //the following three else if / else conditions are exact copies from rook and bishop valid_move_shape
  else if (abs(xdiff) == abs(ydiff)) {
    xdirection = xdiff / abs(xdiff);
    ydirection = ydiff / abs(ydiff);
    for (int i = 0; i <= abs(xdiff); i++) {
      trajectory.push_back(Position(start.x + i * xdirection, start.y + i * ydirection));
    }
  }
  else {
    if (xdiff != 0) {
      xdirection = xdiff / abs(xdiff);
      ydirection = 0;
    } else if (ydiff != 0) {
      ydirection = ydiff / abs(ydiff);
      xdirection = 0;
    }
    for (int i = 0; i <= max(abs(xdiff), abs(ydiff)); i++) {
      trajectory.push_back(Position(start.x + i * xdirection, start.y + i * ydirection));
    }
  }
  
  return SUCCESS;
}
