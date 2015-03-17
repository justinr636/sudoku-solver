// Project 3B
// 
// by Justin Ratra and Eric Dusel
// 
// main.cpp
// main program file
// 

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

// Declarations and functions for project #4
typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank
 
const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;


#include "board.h"

int main()
{
   ifstream fin;
   
   // Read the sample grid from the file.
   //string fileName = "sudoku.txt";
   string fileName = "/Users/Justin/Documents/NEU/Optimization/Project 3/Project3B/project3b/sudoku3.txt";
   //string fileName = "sudoku3.txt";

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      board b1(SquareSize);

      while (fin && fin.peek() != 'Z')
      {
		b1.initialize(fin);

	 	b1.print();

		b1.printConflicts(0);	// print Row Conflicts
		b1.printConflicts(1);	// print Col Conflicts
		b1.printConflicts(2);	// print Sqr Conflicts

		b1.solve();

		// Check if board is solved
		if(b1.isSolved())
		{
			cout << "Sudoku Solved" << endl;
			//b1.printNumberOfRecursions();
			b1.print();
		}
		else
		{
			cout << "Impossible to find Sudoku Solution" << endl;
		}
      }
   }
   catch  (indexRangeError &ex)
   {
      cout << ex.what() << endl;
      exit(1);
   }

   system("pause");
   exit(0);
}
