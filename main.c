/*
 * Final project(4) for the PHY2027 module
 * Date: 14/12/2018
 */

/*
 * This program recreates Conway's game of life which is a basic simulation following specific rules. The program has a
 * variety of choices for how the simulation can be run such as entering a custom pattern, changing how long the
 * simulation runs and the boundary conditions.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ALIVE 'X'
#define DEAD '_'


char **init_board(int height, int width);

void make_template(int height, int width);

void make_board(char **p, int height, int width, int option);

void print_board(char **p, int height);

void evolve_board(char **p, int height, int width, int boundary);

void sleep(int milliseconds);


int main() {

    int option, width = 1, height = 1;
    do {
        printf("Please choose an option:\n"
               "0 - Custom Template\n"
               "1 - Preset 1 (Toad)\n"
               "2 - Preset 2 (Pentadecathlon)\n"
               "3 - Preset 3 (Gospel Glider)\n");

        scanf("%d", &option);

        switch (option) {
            case 0:
                printf("Please enter a width and height for the board with a space in between\n");
                scanf("%d %d", &width, &height);
                make_template(height, width);
                printf("Please edit the template.txt file to input a pattern.\n"
                       "Dead cells are represented with: '%c', Alive cells are represented with: '%c'\n"
                       "Type anything to continue\n", DEAD, ALIVE);
                scanf("%*s");
                break;
            case 1:
                width = 10;
                height = 6;
                break;
            case 2:
                width = 11;
                height = 18;
                break;
            case 3:
                width = 78;
                height = 21;
                break;
            default:
                printf("Not a valid option\n");
                break;
        }
    } while(option < 0 || option > 3);

    int lifetime = 0;
    do {
        printf("How long would you like to run the simulation for:\n");
        scanf("%d", &lifetime);
        if(lifetime < 0){
            printf("Please enter a valid number\n");
        }
    } while(lifetime < 0);

    int boundary;
    do {
        printf("Please selected a boundary condition to use for the simulation:\n"
               "0 - Dead cell border\n"
               "1 - Periodic\n");
        scanf("%d", &boundary);
        if(boundary < 0 || boundary > 1){
            printf("Please enter a valid option\n");
        }
    } while(boundary < 0 || boundary > 1);

    int refresh;
    do {
        printf("Please enter a refresh rate in milliseconds:\n");
        scanf("%d", &refresh);
        if(refresh < 0){
            printf("Please enter a valid number\n");
        }
    } while(refresh < 0);

    char **board = init_board(height, width);
    make_board(board, height, width, option);

    system("cls||clear"); //Fairly certain this is a portable way to clear the screen on any OS

    int index = 0;
    while(index < lifetime) {
        print_board(board, height);
        evolve_board(board, height, width, boundary);
        ++index;
        printf("Frame: %d \n", index);
        sleep(refresh);
        system("cls||clear");
    }

    return 0;
}


/*
 * This function is used to dynamically allocate memory for a 2D array of pointers that will represent the board
 */
char **init_board(int height, int width){
    char **board = NULL;
    board = (char **) calloc(height, sizeof(char *));
    for(int i = 0; i < height; ++i)
        board[i] = (char *) calloc(width+1, sizeof(char)); //+1 as if inputting string terminating character needs space
    return board;
}


/*
 * This function is used to create a text file that can be easily edited to input a custom pattern into the program
 */
void make_template(int height, int width){
    FILE *infile;
    infile = fopen("template.txt", "w");
    char line[width+1];
    for(int i = 0; i < width; ++i){
        line[i] = DEAD;
    }
    line[width] = '\0';
    for(int j = 0; j < height; ++j){
        fprintf(infile, "%s \n", line);
    }
    fclose(infile);
}


/*
 * This function assigns the correct character for each element in the 2D array
 */
void make_board(char **p, int height, int width, int option){
    FILE *infile;
    switch(option){
        case 0:
            infile = fopen("template.txt", "r");
            break;
        case 1:
            infile = fopen("preset1.txt", "r");
            break;
        case 2:
            infile = fopen("preset2.txt", "r");
            break;
        case 3:
            infile = fopen("preset3.txt", "r");
            break;
        default:
            infile = fopen("preset1.txt", "r");
    }


    for(int j = 0; j < height; ++j){
        char row[width+1];
        fscanf(infile, "%s", row);
        for(int i = 0; i < width; ++i){
            p[j][i] = row[i];
        }
    }

    fclose(infile);
}


/*
 * This function is used to print the board on the screen so that it can be seen
 */
void print_board(char **p, int height){
    for(int j = 0; j < height; ++j){
        printf("%s\n", p[j]);
    }
    printf("\n");
}


/*
 * This function is used to determine whether a specific cell should be alive or dead according to the standard rules of
 * Conway's game of life
 */
char rules(char p[3][3]){
    char cell = p[1][1];
    int alive = 0;

    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            if(p[j][i] == ALIVE){
                    ++alive;
            }
        }
    }

    if(alive == 3){
        cell = ALIVE;
    }
    if((alive < 3) || (alive > 4)){
        cell = DEAD;
    }

    return cell;
}


/*
 * This function is used to create array indices that are in the correct range for periodic boundary conditions
 */
int array_index(int dimension, int a){
    int b;
    if(a < 0){
        b = dimension-1;
    }
    else if(a >= dimension-1){
        b = 0;
    }
    else{
        b = a;
    }
    return b;
}


/*
 * This function is used to loop through the board and update it based on the rules
 */
void evolve_board(char **p, int height, int width, int boundary){
    char tmp_board[height][width];
    for(int j = 0; j < height; ++j){
        for(int i = 0; i < width; ++i){
            tmp_board[j][i] = p[j][i];
        }
    }

    char neighbours[3][3];

    for(int j = 0; j < height; ++j){
        for(int i = 0; i < width; ++i){
            for(int y = j-1; y < j+2; ++y){
                for(int x = i-1; x < i+2; ++x){
                    if(y < 0 || y >= height || x < 0 || x >= width){
                        if(boundary == 0){
                            neighbours[y-j+1][x-i+1] = DEAD;
                        }
                        if(boundary == 1){
                            neighbours[y-j+1][x-i+1] = tmp_board[array_index(height, y)][array_index(width, x)];
                        }
                    }
                    else{
                        neighbours[y-j+1][x-i+1] = tmp_board[y][x];
                    }
                }
            }
            p[j][i] = rules(neighbours);
        }
    }
}


/*
 * This function is used to pause the program by a specific number of milliseconds.
 */
void sleep(int milliseconds){
    time_t end = milliseconds + clock();
    while(clock() < end);
}

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Results~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Please choose an option:
0 - Custom Template
1 - Preset 1 (Toad)
2 - Preset 2 (Pentadecathlon)
3 - Preset 3 (Gospel Glider)
1
How long would you like to run the simulation for:
5
Please selected a boundary condition to use for the simulation:
0 - Dead cell border
1 - Periodic
0
Please enter a refresh rate in milliseconds:
50

__________
__________
__XXX_____
___XXX____
__________
__________

Frame: 1







___X______
__X__X____
__X__X____
____X_____
__________

Frame: 2







__________
__XXX_____
___XXX____
__________
__________

Frame: 3







___X______
__X__X____
__X__X____
____X_____
__________

Frame: 4







__________
__XXX_____
___XXX____
__________
__________

Frame: 5

 */