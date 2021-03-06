GPP := g++ -O3 -std=c++11 -static
# GPP := g++ -std=c++11
# CC := gcc

all: unit_test solve_puzzle

utils.hpp.gch: utils.hpp
	${GPP} -c utils.hpp

unit_test: unit_test.cpp utils.hpp.gch MoveGener.hpp MoveClassifier.hpp MoveFilter.hpp MovePlayer.hpp MinMax.hpp
	${GPP} -o unit_test unit_test.cpp -lpthread

solve_puzzle: solve_puzzle.cpp utils.hpp.gch MoveGener.hpp MoveClassifier.hpp MoveFilter.hpp MovePlayer.hpp MinMax.hpp
	${GPP} -o solve_puzzle solve_puzzle.cpp -lpthread

clean: 
	rm *.gch
