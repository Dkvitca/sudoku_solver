//Sudoku Solver in C 
//By Dan Kvitca.

// TODO: Add a puzzle generator to make original puzzles, then feed those back into the program and solve them
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define DIM 9
#define SUBGRID_DIM  3


typedef struct puzzle_t {
        int values[DIM][DIM];
        bool isFixed[DIM][DIM];
        bool subgridHasVal[SUBGRID_DIM][SUBGRID_DIM][DIM];
        bool colHasVal[DIM][DIM];
        bool rowHasVal[DIM][DIM];
        
}Puzzle;

/* Constructor for struct Puzzle, loads puzzle from text and assigns correct values for nums 1-9 */
Puzzle* loadPuzzle(char* fileName) {

        Puzzle* p = (Puzzle*)calloc(1, sizeof(Puzzle));
        if (p == NULL){
                printf("ERROR: Could not allocate memory for puzzle P\n");
                exit(-1);
        }

        FILE *fp;
        fp = fopen(fileName, "r");

        if (fp == NULL){
                printf("ERROR: File %s could not be opened\n", fileName);
                exit(-1); 
        }

        int puzzleVal;
        int i = 0;
        
        // Could maybe change this around to use fread to read the puzzle into values[][]
        // but I don't think it would really be much of an improvement since it would just 
        // have to loop through values[][] anyways to set the bool arrays
        while((puzzleVal = fgetc(fp)) != EOF){
                if (puzzleVal >= 48 && puzzleVal <=57){
                        int row = i / 9;
                        int col = i % 9;
                        int val = puzzleVal - 48;

                        p->values[row][col] = val;
                        
                        if(val > 0){
                                p->isFixed[row][col]   = true;
                                p->rowHasVal[row][val] = true;
                                p->colHasVal[col][val] = true;
                                p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val] = true;
                                }
 
                        ++i;
                } 
        }


        fclose(fp);
        return p;
}

/* Functions for checking the safety of a given square */
int isSafe(Puzzle* p, int val, int row, int col)
{
        return (!(p->rowHasVal[row][val])
                && !(p->colHasVal[col][val])
                && !(p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val])
                && !(p->isFixed[row][col])                );
}

void placeVal(Puzzle* p, int val, int row, int col){

        p->values[row][col] = val;
        p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val] = true;
        p->rowHasVal[row][val] = true;
        p->colHasVal[col][val] = true;
}

void removeVal(Puzzle* p, int val, int row, int col) {
        p->values[row][col] = 0;
        p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val] = false;
        p->rowHasVal[row][val] = false;
        p->colHasVal[col][val] = false;

}

/* The main recursive backtracking function */
int solvePuzzle(Puzzle* p, int n){

        if (n == 81){
                return true;
        }

        int row = n / 9;
        int col = n % 9;

        if (p->isFixed[row][col]){
                if(solvePuzzle(p, n + 1)){
                        return true;
                }
        }

        for (int val = 1; val <= 9; ++val){
                if (isSafe(p, val, row, col)){
                        placeVal(p, val, row, col);

                        if (solvePuzzle(p, n + 1)){
                                return true;
                        }

                        removeVal(p, val, row, col);
                }
        }
        return false;
}
                
/* A wrapper function for calling solvePuzzle() in main() 
   since there will never be a reason to use any number
   other than 0 for n in solvePuzzle()
*/
int WRAPPER_Solve(Puzzle* p) {
        int found = solvePuzzle(p, 0);
        return found;
}

/* A function to print the passed puzzle */
void printPuzzle(Puzzle* p) {
        for(int row = 0; row < 9; ++row){
                for (int col = 0; col < 9; ++col){
                        printf("%i ", p->values[row][col]);
                }
                putchar('\n');
        }
}

int main(int argc, char** argv) {
 
        Puzzle* p = loadPuzzle(argv[1]);
        printPuzzle(p);

        if(WRAPPER_Solve(p) == 1){
                printf("\nA solution has been found!\n\n");
                printPuzzle(p);
        } else {
                printf("Something has gone wrong, here is the output:\n");
                printPuzzle(p);
        }

        return 0;
}