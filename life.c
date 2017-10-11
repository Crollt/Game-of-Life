#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include "life.h"

int main(int argc, const char *argv[]) {
    initscr();
    /* raw(); */
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    /* ------------------------------ */
    Grid grid;
    init_grid(&grid, 50, 150);

    make_glider(&grid, 5, 50);

    while (1) {
        print_grid(&grid);
        run_game(&grid);
        usleep(500000);
    }
    /* ------------------------------ */
    getch();
    endwin();
    return 0;
}

void print_grid(Grid* grid) {
    clear();
    for (int i = 0; i < grid->size_x; i++) {
        for (int j = 0; j < grid->size_y; j++) {
            if (grid->matrix[i][j] == 0) {
                printw(" ");
            } else {
                printw("0");
            }
        }
        printw("\n");
    }
    refresh();
}

void init_grid(Grid* grid, int x, int y) {
    grid->size_x = x;
    grid->size_y = y;
    
    grid->matrix = calloc(x, sizeof(int *));
    for (int i = 0; i < x; i++) {
        grid->matrix[i] = calloc(y, sizeof(int));
    }
}

Grid* deep_copy_grid(Grid* grid) {
    Grid* copy; 
    copy = malloc(sizeof(Grid));
    init_grid(copy, grid->size_x, grid->size_y);
    for (int i = 0; i < grid->size_x; i++) {
        for (int j = 0; j < grid->size_y; j++) {
            copy->matrix[i][j] = grid->matrix[i][j];
        }    
    }
    return copy;
}

void make_glider(Grid* grid, int x, int y) {
    int trans[5][2] = {
        {0, 1},
        {1, 2},
        {2, 0},
        {2, 1},
        {2, 2}
    };
    for (int i = 0; i < 5; i++) {
        grid->matrix[x + trans[i][0]][y + trans[i][1]] = 1;
    }
}

void run_game(Grid* grid) {
    int adjacent = 0;
    Grid* copy = deep_copy_grid(grid);
    for (int i = 0; i < grid->size_x; i++) {
        for (int j = 0; j < grid->size_y; j++) {
            adjacent = count_adjacent(grid, i, j); 
            if (grid->matrix[i][j] == 1) {
                if (adjacent < 2) {
                    copy->matrix[i][j] = 0;
                }
                if (adjacent >= 4) {
                    copy->matrix[i][j] = 0;
                }
            } else {
                if (adjacent == 3) {
                    copy->matrix[i][j] = 1;
                }
            }
        }
    }   
    for (int i = 0; i < grid->size_x; i++) {
        for (int j = 0; j < grid->size_y; j++) {
            grid->matrix[i][j] = copy->matrix[i][j]; 
        }   
    }
}

int count_adjacent(Grid* grid, int i, int j) {
    int adjacent_squares[8][8] = {
        {-1, -1},
        {-1, 0},
        {0, -1},
        {1, 1},
        {1, 0},
        {0, 1},
        {-1, 1},
        {1, -1},
    };
    
    int adjacent = 0;
    for (int k = 0; k <8; k++) {
        int in_x = i + adjacent_squares[k][0];
        int in_y = j + adjacent_squares[k][1];
        if (!(in_x <= -1 || in_y <= -1 || in_x >= grid->size_x || in_y >= grid->size_y)) {
            if (grid->matrix[in_x][in_y] == 1) {
                adjacent++;
            }
        }
    }
    return adjacent;
}

