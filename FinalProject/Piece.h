#ifndef PIECE_H
#define PIECE_H

#include "Enumerations.h"
//
#include <vector>
//

// Forward declaration of Piece class, present here so classes above 
// the Piece class definition in this file can refer to Piece as a type.
class Piece;


// Piece type enumeration.
enum PieceEnum {
  PAWN_ENUM = 0,
  ROOK_ENUM,
  KNIGHT_ENUM,
  BISHOP_ENUM,
  QUEEN_ENUM,
  KING_ENUM,
  GHOST_ENUM  //ghost piece used in Spooky Chess only
};


// A (virtual) class responsible for creating new instances of a
// particular type of piece (factory pattern).
class AbstractPieceFactory {
public:
    // Create a piece with the specified owner
    virtual Piece* new_piece(Player owner) const = 0;
    virtual ~AbstractPieceFactory() {}
};


// A templated class generating Pieces.
template <class T>
class PieceFactory : public AbstractPieceFactory {

public:
    PieceFactory(int piece_type) : _piece_type(piece_type) {}
    Piece* new_piece(Player owner) const override {
        return new T(owner, _piece_type);
    }

protected:
    int _piece_type;
};



// Class representing an individual piece
class Piece {

public:

    // There is no public constructor: only use factories to build this
    // class's subclasses

    // Destructor
    virtual ~Piece() {}

    // Returns the owner of the piece.
    Player owner() const { return _owner; }

    // Returns the piece_type of the piece.
    int piece_type() const { return _piece_type; }

    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const = 0;


protected:
    Player _owner;
    int _piece_type;

    // Constructs a piece with a specified owner
    // Note that this is deliberately made protected. Use the factory only!
    Piece(Player owner, int piece_type) : _owner(owner) , _piece_type(piece_type) {}
};


#endif // PIECE_H
