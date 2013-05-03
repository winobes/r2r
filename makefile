all: main.c
	gcc `pkg-config --cflags gtk+-3.0` -o a main.c `pkg-config --libs gtk+-3.0`
