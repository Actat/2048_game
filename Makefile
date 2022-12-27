test:	main.o game_board.o game_master.o player_game_tree.o game_board_eval.o
	g++ -o test.out main.o game_board.o game_master.o player_game_tree.o game_board_eval.o
eval:	evaluate_player.o game_board.o game_master.o player_game_tree.o game_board_eval.o
	g++ -o eval.out evaluate_player.o game_board.o game_master.o player_game_tree.o game_board_eval.o
main.o:	main.cpp
	g++ -c main.cpp
game_board.o:	game_board.cpp
	g++ -c game_board.cpp
game_board_eval.o: game_board_eval.cpp
	g++ -c game_board_eval.cpp
game_master.o:	game_master.cpp
	g++ -c game_master.cpp
player_game_tree.o:	player_game_tree.cpp
	g++ -c player_game_tree.cpp

.PHONY: clean
clean:
	rm -f main.o evaluate_player.o game_board.o game_master.o player_game_tree.o game_board_eval.o test.out eval.out
