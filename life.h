#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

typedef struct {
    int size_x, size_y;
    int** matrix;
} Grid;

void print_grid(Grid*);
void init_grid(Grid*, int, int);
void make_glider(Grid*, int, int);
void run_game(Grid*);
int count_adjacent(Grid*, int, int);
Grid* deep_copy_grid(Grid*);
void read_file(Grid*, char*);
void free_grid(Grid*);
