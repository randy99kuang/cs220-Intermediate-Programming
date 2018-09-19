CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

# play target
play: Play.o Game.o ChessGame.o ChessPiece.o KingGame.o SpookyGame.o
	$(CXX) Play.o Game.o ChessGame.o ChessPiece.o KingGame.o SpookyGame.o -o play

Play.o: Play.cpp Game.h ChessGame.h Prompts.h KingGame.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c Play.cpp

Game.o: Game.cpp Game.h Piece.h Prompts.h Enumerations.h Terminal.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

ChessGame.o: ChessGame.cpp Game.h ChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c ChessGame.cpp

ChessPiece.o: ChessPiece.cpp ChessPiece.h Game.h #Enumerations.h Piece.h
	$(CXX) $(CXXFLAGS) -c ChessPiece.cpp

KingGame.o: KingGame.cpp KingGame.h ChessGame.h Prompts.h
	$(CXX) $(CXXFLAGS) -c KingGame.cpp

SpookyGame.o: SpookyGame.cpp SpookyGame.h Prompts.h ChessGame.h #ChessPiece.h
	$(CXX) $(CXXFLAGS) -c SpookyGame.cpp


# test target
test: ChessPiece.o
	$(CXX) ChessPiece.o -o test


# clean up
clean:
	rm *.o play

