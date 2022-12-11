test:	main.o game_board.o game_master.o player_minimax.o
	g++ -o test.out main.o game_board.o game_master.o player_minimax.o
eval:	evaluate_player.o game_board.o game_master.o player_minimax.o
	g++ -o eval.out evaluate_player.o game_board.o game_master.o player_minimax.o
main.o:	main.cpp
	g++ -c main.cpp
game_board.o:	game_board.cpp
	g++ -c game_board.cpp
game_master.o:	game_master.cpp
	g++ -c game_master.cpp
player_minimax.o:	player_minimax.cpp
	g++ -c player_minimax.cpp
