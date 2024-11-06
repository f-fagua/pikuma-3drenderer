build:
	mkdir -p bin
	gcc -Wall -std=c99 ./src/*.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lm -o bin/renderer

run:
	./bin/renderer

clean:
	rm bin/renderer