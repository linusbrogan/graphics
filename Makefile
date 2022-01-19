.PHONY: all clean gs

all: clean gs

gs: growing_sun movie_player
	./growing_sun 600 400 100 1.5
	./Growing_Sun/play_movie.sh

movie_player:
	cc -o movie_player movie_player.c -lm -lX11

growing_sun:
	cc -o growing_sun growing_sun.c -lm -lX11

clean:
	-rm growing_sun movie_player
	-rm -r Growing_Sun