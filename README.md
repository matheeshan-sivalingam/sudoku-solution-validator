# Sudoku Solution Validator

## Introduction
For this lab, we were tasked to design a multithreaded application that determines whether the
solution to a Sudoku puzzle is valid. A sudoku solver algorithm was first implemented to solve
the sudoku puzzle written on Lab3 puzzle.txt. This algorithm did not use threading to solve the
puzzle however a multithreaded approach was implemented when validating the solution.

## Design and implementation
To solve the sudoku puzzle, a recursive backtracking algorithm was implemented to find a valid
number to place in the cell. The following functions were used to implement this:
<li>getBoard() - This function reads the Lab3 puzzle.txt file and stores the numbers into a
board double-pointer.<li>solveBoard() - This function attempts to solve the puzzle by putting a number onto a cell
that is equal to 0 and checking to see if it is a valid solution by seeing if the number is
unique in its row, column, and 3x3 box. If the solution is valid, then it would move to the
next column in the row
<li>rowCheck() - This function takes the potential number and row number to determine if
the solution is unique in its row
<li>colCheck() - This function takes the potential number and column number to determine if
the solution is unique in its column
<li>boxCheck() - This function takes the potential number, row number, and column number,
determines if the number is unique in its 3x3 subgrid
  
To verify if this solution was valid, 11 threads are created to check every row, every column, and
every sub-grid. Two threads are used to check to see if the row and column are valid. The nine remaining threads are used to check if each subgrid is valid. To pass the parameters to the threads, a sudokuPuzzle data structure was created that holds the solved board as well as the
board number (bNum). Each board number represents a different subgrid of the board and is
used to determine what subgrid to check. The following functions are used by the threads to
ensure the validity of the solution:
  
<li>tRowCheck() - This function ensures that each row in the solution is valid as it checks if
every number in each row is unique.
<li>tColCheck() - This function ensures that each column in the solution is valid as it checks
if every number in each column is unique
<li>tBoxCheck() - This function ensures that each 3x3 subgrid in the solution is valid by
checking if every number in the subgrid is unique.
  
All functions either return a value of one indicating that there is a repeated number in the
row/column/subgrid, therefore, rendering the solution invalid, or a value of zero which indicates
that row/column/subgrid is valid.
  
  
  
  
  
  
  
