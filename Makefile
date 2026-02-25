main: main.c player.c enemy.c powerup.c
	gcc main.c player.c enemy.c powerup.c -o main -lraylib -ggdb

clean: 
	rm main
