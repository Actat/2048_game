test:	main.o game_board.o game_master.o
	g++ -o test.out main.o game_board.o game_master.o
main.o:	main.cpp
	g++ -c main.cpp
game_board.o:	game_board.cpp
	g++ -c game_board.cpp
game_master.o:	game_master.cpp
	g++ -c game_master.cpp
