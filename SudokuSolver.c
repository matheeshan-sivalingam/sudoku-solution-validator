// Name: Matheeshan Sivalingam (100703887)
// Program: SudokuSolver.c
// Date: 23/03/2022

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#define BUFFER_LEN 256
//Print board function (mainly used for testing)
int printBoard(int **board){
    //Use for loop to iterate through rows
    for (int i = 0;i<9;i++)
        {
            //Use for loop to iterate through column
            for (int j=0;j<9;j++)
            {
                //Print number on board
                printf("%d ",board[i][j]);
            }
            //Break space for each row
            printf("\n");
        }
}
//getBoard function - used to read from the txt file and get sudokuboard from txt file
int **getBoard()
{
    //Declare pointer to board pointer
    int **board;
    //Declare file pointer
    FILE *puzzle = fopen("Lab3 puzzle.txt","r");
    //Declare digits array to hold digits obtained from txt file
    int digits[9][9];
    //Declare readFile variable used to store file input
    char readFile[BUFFER_LEN];
    //If file is found (not null)
    if (puzzle != NULL)
    {
        //Declare i counter variable to hold row count 
        int i = 0;
        //Use while loop to read through entire file
        while (fgets(readFile, BUFFER_LEN, puzzle))
        {
            //Declare j counter variable to hold column count
            int j = 0;
            //Declare k variable to hold line count
            int k = 0;
            //Loop until end character
            while (readFile[k]!='\0')
            {
                //If it is not space
                if (isspace(readFile[k])==0)
                {
                    //Obtain digit by converting char value to int 
                    digits[i][j] = readFile[k]-'0';
                    //Increment line counter
                    j++;
                }
                //Increment column count
                k++;
            }
            //Increment row count
            i++;
        }
    }
    // If the file can't be found, print that the file can't be found
    else
    {
        //Print that the file can't be found
        printf("File can't be found");
    }
    
    //Allocate memory for sudoku boards
    board = (int**)malloc(sizeof(int*)*9);
    //Declare i variable to hold row count
    int i;
    //Declare j variable to hold column count
    int j;

    //Use for loop to allocate memory. 
    for (i=0;i<9;i++)
    {
        //Allocate memory for each sudoku board
        board[i] = (int*)malloc(sizeof(int)*9);
        //Use for loop to fill board with values from digits
        for (j=0;j<9;j++)
        {
            //Set board values to digit values
            board[i][j] = digits[i][j];
        }
    }
    //Return board
    return board;
}
//solveBoard function - Used to find solution of Sudoku puzzle from txt file
int solveBoard(int **board,int row, int col)
{
    //If it is on the last digit on the board, return to main
    if (row==8 && col==8)
    {
        //Return 1
        return 1;    
    } 

    //If it is on the last digit of the column, then go down to next row and reset column
    if (col == 9)
    {
        //Increment row
        row++;
        //Reset column
        col = 0;
    } 
    
    //If the digit is zero
    if (board[row][col]==0)
    {
        //For loop to try each number
        for (int i =1;i<=9;i++)
        {
            //Call row check function using potential number
            int rowC = rowCheck(board,row,i);
            //Call column check function using potential number
            int colC = columnCheck(board,col,i);
            //Call box check function using potential number
            int boxC = boxCheck(board,row,col,i);

            //If all functions return 1
            if (rowC==1 && colC==1 && boxC==1)
            {
                //Set the number as a solution 
                board[row][col] = i;
                //Use a recursive call to get next solution for the next number. If the number is a valid solution 
                if (solveBoard(board,row,col+1)==1)
                {
                    //Return 1
                    return 1;
                }
            }
            //Set number to zero if no solution is found 
            board[row][col] = 0;
        }
        
    }
    else
    {
        //Use a recursive call to go to next column
        return solveBoard(board,row,col+1);
    }


} 

//Row check - Iterates through row to see if potential number is a valid solution
int rowCheck(int **board, int row,int i)
{
    //Use for loop to iterate through row
    for (int j = 0;j<9;j++)
    {
        //If the potential number exists in the row
        if (board[row][j] == i)
        {
            //Return 0
            return 0;
        }
    }
    //Return 1 if the number is unique in row
    return 1;
}

//ColumnCheck function - Iterates through column to see if potential number is a valid solution
int columnCheck(int **board, int col,int i)
{
    //Use for loop to iterate through column
    for (int j = 0;j<9;j++)
    {
        //If the potential number exists in the column
        if (board[j][col] == i)
        {
            //Return 0
            return 0;
        }
    }
    //Return 1 if the number is unique in column
    return 1;
}

//BoxCheck function - Iterates through box (3x3) to see if potential number is a valid solution
int boxCheck(int **board, int row,int col,int i)
{
    //Declare box row/column variables 
    int bRow = row - row %3;
    int bCol = col - col%3;
    //Use for loop to iterate through each row in box
    for (int j = 0;j<3;j++)
    {
        //Use for loop to iterate through each column in box
        for (int k = 0;k<3;k++)
        {
            //If the value is equal to i 
            if (board[j+bRow][k+bCol] == i){
                //Return 0 
                return 0;
            }
        }
    }
    //Return 1 if the number is unique in box
    return 1;
}
//Define datatype to pass board to threads 
typedef struct sudokuPuzzle
{
    //Declare board variable
    int **board;
    //Declare board number variable
    int bNum;

} sudokuPuzzle;

//tRowCheck function - Thread function that verifies that all numbers in the row are unique
void* tRowCheck(void *arg) {
    //Declare and define struct based on parameters sent
    struct sudokuPuzzle *p = arg;
    //Declare and initialize row check pointer 
    int *rowC = malloc(sizeof(int));
    //Use for loop to iterate through each row in board
    for (int i =0;i<9;i++)
    {
        //Declare and initialize usedDigits array. This stores all digits that are used in the row
        int usedDigits[9] = {0,0,0,0,0,0,0,0,0,0};
        //Use for loop to iterate through each digit in row
        for (int j = 0;j<9;j++)
        {
            //Declare and initialize number variable to value of digit on specified position
            int n = p->board[i][j];
            //Use for loop to iterate through use
            for (int k = 0;k<9;k++)
            {
                //If the number has been used
                if (usedDigits[k]==n)
                {
                    //Set row check to 1
                    *rowC = 1;
                    //Return to parent thread with rowC
                    return (void*) rowC;
                }
            }
            //If the digit is unique, add it to the usedDigits array
            usedDigits[j] = n;
        }
    }

    //If all digits are unique, set rowC to 0
    *rowC = 0;
    //Return to parent thread with rowC
    return (void*) rowC;
    //Exit thread
    pthread_exit(NULL);
}
//tRowCheck function - Thread function that verifies that all numbers in the column are unique
void* tColCheck(void *arg) {
    //Declare and define struct based on parameters sent
    struct sudokuPuzzle *p = arg;
    //Declare and allocate memory for column check variable
    int *colC = malloc(sizeof(int));
    //Use for loop to iterate through all columns on board
    for (int i =0;i<9;i++)
    {
        //Declare usedDigit array
        int usedDigits[9] = {0,0,0,0,0,0,0,0,0,0};
        //Use for loop to iterate through each number in column
        for (int j = 0;j<9;j++)
        {
            //Set number variable to specified digit
            int n = p->board[j][i];
            //Use for loop to iterate through usedDigits array
            for (int k = 0;k<9;k++)
            {
                //If the number is found in the usedDigits array
                if (usedDigits[k]==n)
                {
                    //Set column Check to 1
                    *colC = 1;
                    //Return to parent thread 
                    return (void*) colC;
                }
            }
            //If the number was not used, add it to the usedDigits array
            usedDigits[j] = n;
        }
    }
    
    //Set column check to 0
    *colC = 0;
    //Return to parent thread
    return (void*) colC;
    //Exit thread
    pthread_exit(NULL);
}
//tBoxCheck function - Thread function that verifies that all numbers in the 3x3 box are unique
void* tBoxCheck(void *arg) {
    //Declare and define struct based on parameters sent
    struct sudokuPuzzle *p = arg;
    //Declare and allocate memory for box check variable
    int *boxC = malloc(sizeof(int));
    //Declare usedDigits array 
    int usedDigits[9] = {0,0,0,0,0,0,0,0,0,0};
    //Declare m counter to keep track of usedDigits array
    int m = 0;
    //If the board number is one (top left)
    if (p->bNum==1)
    {
        //Set the rowLimitation based for board number
        int rowLim = 3;
        int colLim = 3;
        //Use for loops to iterate through box
        for (int i=0;i<rowLim;i++)
        {
            for (int j=0;j<colLim;j++)
            {
                //Set the number to the specified board digit value
                int n = p->board[i][j];
                //Use for loop to iterate through usedDigits array 
                for (int k = 0;k<9;k++)
                {
                    //If the number is found in the usedDigits array
                    if (usedDigits[k]==n)
                    {
                        //Set boxCheck to 1 and return to parent thread
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                //Set usedDigits to the number
                usedDigits[m] = n;
                //Iterate usedDigits counter
                m++;
            }
        }
    }
    //If the board number is 2 (top center) (Essentially do the same thing as for board number 1 function however adjust row/col limitations)
    else if (p->bNum==2)
    {
        int rowLim = 3;
        int colLim = 6;
        int m = 0;
        for (int i=0;i<rowLim;i++)
        {
            for (int j=3;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    //If the board number is 3 (top right), check top right box to see if all numbers are unique
    else if (p->bNum==3)
    {
        int rowLim = 3;
        int colLim = 9;
        int m = 0;
        for (int i=0;i<rowLim;i++)
        {
            for (int j=6;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    //If the board number is 4 (middle left), check to see if all numbers are unique in box
    else if (p->bNum==4)
    {
        int rowLim = 6;
        int colLim = 3;
        int m = 0;
        for (int i=3;i<rowLim;i++)
        {
            for (int j=0;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    //If the board number is 5 (middle center), check to see if all numbers are unique in box
    else if (p->bNum==5)
    {
        int rowLim = 6;
        int colLim = 6;
        int m = 0;
        for (int i=3;i<rowLim;i++)
        {
            for (int j=3;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    //If the board number is 6 (middle right), check to see if all numbers are unique in box
    else if (p->bNum==6)
    {
        int rowLim = 6;
        int colLim = 9;
        int m = 0;
        for (int i=3;i<rowLim;i++)
        {
            for (int j=6;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    //If the board number is 7 (bottom left), check to see if all numbers are unique in box
    else if (p->bNum==7)
    {
        int rowLim = 9;
        int colLim = 3;
        int m = 0;
        for (int i=6;i<rowLim;i++)
        {
            for (int j=0;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    //If the board number is 8 (bottom center), check to see if all numbers are unique in box
    else if (p->bNum==8)
    {
        int rowLim = 9;
        int colLim = 6;
        int m = 0;
        for (int i=6;i<rowLim;i++)
        {
            for (int j=3;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    //If the board number is 9 (bottom right), check to see if all numbers are unique in box
    else if (p->bNum==9)
    {
        int rowLim = 9;
        int colLim = 9;
        int m = 0;
        for (int i=6;i<rowLim;i++)
        {
            for (int j=6;j<colLim;j++)
            {
                int n = p->board[i][j];
                for (int k = 0;k<9;k++)
                {
                    if (usedDigits[k]==n)
                    {
                        *boxC = 1;
                        return (void*) boxC;
                    }
                }
                usedDigits[m] = n;
                m++;
            }
        }
    }
    
    //If all values in box are unique, set boxc to zero
    *boxC = 0;
    //Return to parent thread
    return (void*) boxC;
    //Exit thread
    pthread_exit(NULL);
    
}

int main (int argc, char *arg[])
{
    //Declare double pointer for board and initialze value based on getBoard function 
    int **board = getBoard();
    //Use solveBoard function to solve Sudoku Puzzle
    int s = solveBoard(board,0,0);
    //If there is a solution, output the board
    if (s==1)
    {
        printBoard(board);
    }
    //Declare check variables (row check, column check, box check, and box flag)
    int *rowC;
    int *colC;
    int *boxC[9];
    int boxF = 0;

    //Declare file pointer
    FILE *fptr = fopen("solution.txt","w");

    //Declare sudokuPuzzle data structure for main board (9x9)
    sudokuPuzzle p0;
    //Declare sudokuPuzzle data structures for each 3x3 board
    sudokuPuzzle p1;
    sudokuPuzzle p2;
    sudokuPuzzle p3;
    sudokuPuzzle p4;
    sudokuPuzzle p5;
    sudokuPuzzle p6;
    sudokuPuzzle p7;
    sudokuPuzzle p8;
    sudokuPuzzle p9;
    
    //Set board value for p0 to board
    p0.board = board;
    
    //Set board value for p1 to board and board number to 1
    p1.board = board;
    p1.bNum = 1;

    //Set board value for p2 to board and board number to 2
    p2.board = board;
    p2.bNum = 2;

    //Set board value for p3 to board and board number to 3
    p3.board = board;
    p3.bNum = 3;

    //Set board value for p4 to board and board number to 4
    p4.board = board;
    p4.bNum = 4;

    //Set board value for p5 to board and board number to 5
    p5.board = board;
    p5.bNum = 5;
    
    //Set board value for p6 to board and board number to 6
    p6.board = board;
    p6.bNum = 6;

    //Set board value for p7 to board and board number to 7
    p7.board = board;
    p7.bNum = 7;

    //Set board value for p8 to board and board number to 8
    p8.board = board;
    p8.bNum = 8;

    //Set board value for p9 to board and board number to 9
    p9.board = board;
    p9.bNum = 9;

    //Declare thread IDs for row check, column check, and box check threads
    pthread_t row_c;
    pthread_t col_c;
    pthread_t box_c[9];

    //Call row check thread
    pthread_create(&row_c, NULL, &tRowCheck, &p0);
    //Wait for row check thread to return
    pthread_join(row_c, &rowC);
    //Call col check thread
    pthread_create(&col_c, NULL, &tColCheck, &p0);
    //Wait for row check thread to return
    pthread_join(col_c, &colC);

    //Call box check thread for each box.
    pthread_create(&box_c[0], NULL, &tBoxCheck, &p1);
    pthread_create(&box_c[1], NULL, &tBoxCheck, &p2);
    pthread_create(&box_c[2], NULL, &tBoxCheck, &p3);
    pthread_create(&box_c[3], NULL, &tBoxCheck, &p4);
    pthread_create(&box_c[4], NULL, &tBoxCheck, &p5); 
    pthread_create(&box_c[5], NULL, &tBoxCheck, &p6);
    pthread_create(&box_c[6], NULL, &tBoxCheck, &p7);
    pthread_create(&box_c[7], NULL, &tBoxCheck, &p8);
    pthread_create(&box_c[8], NULL, &tBoxCheck, &p9);

    //Use for loop to wait for all box check threads to return
    for (int i =0;i<9;i++)
    {
        //Wait for box check thread to return
        pthread_join(box_c[i], &boxC[i]);
    }
    
    //Use for loop to check to see if any box flags are raised
    for (int i = 0;i<9;i++)
    {
        //If there is a box that has a repeated number
        if (*boxC[i]==1)
        {
            //Output to the user that it is an invalid solution
            printf("Repeated number at box %d\n",i+1);
            //Raise box flag
            boxF = 1;
        }
    }

    //If the box flag is raised
    if (boxF==1)
    {
        //Print that the solution is invalid
        printf("Solution is invalid\n");
    }
    //If row check is violated
    else if (*rowC==1)
    {
        //Print that the solution is invalid
        printf("Row check violated. Solution is invalid\n");
    }
    //If column check is violated
    else if (*colC==1)
    {
        //Print that the solution is invalid
        printf("Column check violated. Solution is invalid\n");
    }
    //If there are no violations
    else
    {
        //Print that the solution is valid
        printf("Solution is valid\n");

        //Use for loop to iterate through rows
        for (int i = 0;i<9;i++)
            {
                //Use for loop to iterate through column
                for (int j=0;j<9;j++)
                {
                    //Print number on file
                    fprintf(fptr,"%d ",board[i][j]);
                }
                //Break space for each row
                fprintf(fptr,"\n");
            }
    }
    
    //Close file
    fclose(fptr);
    //Return 0 
    return 0;
}