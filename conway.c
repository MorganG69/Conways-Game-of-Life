#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BOARD_X 70
#define BOARD_Y 40

char board1[BOARD_Y][BOARD_X];
char board2[BOARD_Y][BOARD_X];

char (*current_board)[BOARD_X];
char (*next_board)[BOARD_X];

int check_above(int x, int y) {
    int total = 0;
    int xstart = 0;
    int xend = 0;
    int ylim = y-1;
    if(y == 0) {
        return 0;
    } else {
        switch (x) {
            case 0:           
                xstart = 0;
                xend = 2;      
            break;

            case BOARD_X - 1:
                xstart = x-1;
                xend = BOARD_X;
            break;

            default:
                xstart = x-1;
                xend = x+2;
            break;
        }

        for(int i = xstart; i < xend; i++) {
            if(current_board[ylim][i] == '1') {
                total++;
            }
        }
    }
    return total;
}

int check_sides(int x, int y) {
    int total = 0;
    int i = 0;

    if(x < BOARD_X-1) {
        if(current_board[y][x+1] == '1') {
            total++;
        }       
    }

    if(x > 0) {
        if(current_board[y][x-1] == '1') {
            total++;
        }      
    }
    return total;
}

int check_below(int x, int y) {
    int total = 0;
    int xstart = 0;
    int xend = 0;
    int ylim = y+1;
    if(y == BOARD_Y - 1) {
        return 0;
    } else {
        switch (x) {
            case 0:           
                xstart = 0;
                xend = 2;      
            break;

            case BOARD_X - 1:
                xstart = x-1;
                xend = BOARD_X;
            break;

            default:
                xstart = x-1;
                xend = x+2;
            break;
        }

        for(int i = xstart; i < xend; i++) {
            if(current_board[ylim][i] == '1') {
                total++;
            }
        }
    }
    return total;
}

void clear_board(char (*board)[BOARD_X]) {
    for(int y = 0; y < BOARD_Y; y++) {
        for(int x = 0; x < BOARD_X; x++) {
            board[y][x] = '0';
        }
    }
}

void show_current_board(void) {
    for(int i = 0; i <= BOARD_X+1; i++) {
        printf("-");
    }
    printf("\n");
    for(int y = 0; y < BOARD_Y; y++) {
        printf("|");
        for(int x = 0; x < BOARD_X; x++) {
            if(current_board[y][x] == '0') {
                printf(" ");
            } else {
                printf("x");
            }
        }
        printf("|\n");
    }
    for(int i = 0; i <= BOARD_X+1; i++) {
        printf("-");
    }
    printf("\n");
}

void swap_boards(void) {
    char (*tmp)[BOARD_X];
    tmp = current_board;
    current_board = next_board;
    next_board = tmp;
}

void do_generation(void) {
    for(int y = 0; y < BOARD_Y; y++) {
        for(int x = 0; x < BOARD_X; x++) {
            int cell_total = 0;
            cell_total += check_above(x, y);
            cell_total += check_sides(x, y);
            cell_total += check_below(x, y);

            if(current_board[y][x] == '0') {
                if(cell_total == 3) {
                    next_board[y][x] = '1';
                }
            } else {
                if(cell_total < 2 || cell_total > 3) {
                    next_board[y][x] = '0'; 
                } else {
                    next_board[y][x] = '1';
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    FILE *ifp;
    int generations;

    current_board = board1;
    next_board = board2;
    clear_board(current_board);
    clear_board(next_board);
    
    printf("Conways Game of Life in C\n");
    printf("Morgan Greenhill - 2023\n");

    if(argv[2] == NULL) {
        printf("provide number of generations\n");
        return -1;
    } else {
        generations = strtoul(argv[2], NULL, 10);
    }

    ifp = fopen(argv[1], "r");
    if(ifp == NULL) {
        printf("failed to open initial state file\n");
        return -1;
    }

    for(int y = 0; y < BOARD_Y; y++) {
        for(int x = 0; x < BOARD_X; x++) {
            int c = fgetc(ifp);
            if(c != '0' && c != '1') {
                break;
            } else {
                current_board[y][x] = c;
            }
        }
        if(fgetc(ifp) == EOF) {
            break;
        }
    }
    
    printf("Generations: %d\n", generations);

    for(int g = 0; g <= generations; g++) {
        show_current_board();  
        printf("generation: %d\n", g);
        usleep(100000);

        printf("\033[%dA", BOARD_Y+3);
        clear_board(next_board);
        do_generation();
        swap_boards();           
    }
    sleep(5);
    fclose(ifp);
    return 0;
}