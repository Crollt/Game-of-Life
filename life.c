#include "life.h"

int main(int argc, const char *argv[]) {
    float per_second = 0.1;
    char * file_name;

    initscr();
    /* raw(); */
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, 1);
    curs_set(0);
    /* ------------------------------ */
    Grid grid;
    init_grid(&grid, 50, 100);
    if (argv[1]) {
        read_file(&grid, argv[1]);
    } else {
        read_file(&grid, "default.txt");
    }
    /* make_glider(&grid, 5, 50); */

    int ch;
    bool paused = false;
    while (1) {
        ch = getch();
        if (ch == (int)'p' && !paused) {
            paused = true;
        } else if (ch == (int)'p' && paused) {
            paused = false;
        }
        if (!paused) {
            print_grid(&grid);
            run_game(&grid);
            int time = 1000000 * per_second;
            usleep(time);
        } else {
            if (ch == KEY_RIGHT) {
                print_grid(&grid);
                run_game(&grid);
            }
        }
        if (ch == (int)'x') {
            break;
        }
    }
    free_grid(&grid);
    /* ------------------------------ */
    getch();
    endwin();
    return 0;
}

void read_file(Grid* grid, char* file_name) {
    FILE *file;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(file_name, "r");
    if (file) {
        int j = 0;
        char buf[] = "X";
        while((read = getline(&line, &len, file) != -1)) {
            for (int i = 0; i < strlen(line); i++) {
                buf[0] = line[i];
                grid->matrix[j][i] = atoi(buf);
            }
            j++;
        }
    }
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
    free_grid(copy);
    free(copy);
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

void free_grid(Grid* grid) {
    for (int i = 0; i < grid->size_x; i++) {
        free(grid->matrix[i]);
    }
    free(grid->matrix);
}
