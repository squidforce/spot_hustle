NAME = final_vers
CC = g++
CFLAGS = -Wall -g -O0 `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
 
all:
	$(CC) $(CFLAGS) $(LIBS) $(NAME).cpp -o $(NAME)