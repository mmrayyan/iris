CC = gcc
SRC = ./src/main.c ./src/display.c ./src/vector.c ./src/mesh.c ./src/triangle.c ./src/array.c ./src/draw.c ./src/matrix.c ./src/color.c ./src/light.c
OUT = ./build/iris
CFLAGS = -Wall
LDFLAGS = -lmingw32 -lSDL2main -lSDL2

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	$(OUT)

clean:
	rm $(OUT)

.PHONY: all run clean