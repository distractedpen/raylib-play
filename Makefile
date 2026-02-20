main: main.c
	gcc main.c -o main -lraylib -ggdb

clean: 
	rm main
