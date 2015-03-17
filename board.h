// Project 3B
// 
// by Justin Ratra and Eric Dusel
// 
// board.h
// board class header file
// 

class board
// Stores the entire Sudoku board
{
   public:
	  // Constructors
      board(int);
	  
	  // Methods
      void clear();
      void initialize(ifstream &fin);
      void print() const;
      void printConflicts(int) const;
      bool isBlank(int, int) const;
      ValueType getCell(int, int) const;
	  void setCell(int, int, int);

	  bool checkConflict(int, int, int);
	  void updateCell(int, int, int);
	  void clearCell(int, int);
	  bool isSolved() const;
	  void solve();
	  void findFirstBlankCell(int&, int&);
	  void printNumberOfRecursions();

   private:
	   // Properties

      // The following matrices go from 0 to BoardSize in each
      // dimension.  I.e. they are each (BoardSize) X (BoardSize)
      matrix<ValueType> value;

	  vector< vector<bool> > rowConflicts;		// Store Row Conflicts 
	  vector< vector<bool> > colConflicts;		// Store Col Conflicts 
	  vector< vector<bool> > sqrConflicts;		// Store Square Conflicts 

	  int recursionCounter; 					// Count the number of recursions
};

board::board(int sqSize)
   : value(BoardSize,BoardSize)
   , rowConflicts(BoardSize, vector<bool>(BoardSize))
   , colConflicts(BoardSize, vector<bool>(BoardSize))
   , sqrConflicts(BoardSize, vector<bool>(BoardSize))
// Board constructor
{
}

void board::clear()
// Clears all values (and conflicts) in Board
{
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			value[i][j] = 0;

			rowConflicts[i][j] = false;
			colConflicts[i][j] = false;
			sqrConflicts[i][j] = false;
		}
	}
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
   char ch;

   clear();
   for (int i = 0; i < BoardSize; i++)
   {
      for (int j = 0; j < BoardSize; j++)
      {
		  fin >> ch;

     	  // If the read char is not Blank
	 	  if (ch != '.')
	 	  {
     	         setCell(i,j,ch-'0');   // Convert char to int
	 	  }
      }
   }
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
   // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
   // coordinates of the square that i,j is in.  

   return SquareSize * (i/SquareSize) + (j/SquareSize) + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
   for (int i = 0; i < v.size(); i++)
      ostr << v[i] << " ";
   ostr << endl;
   return ostr;
}

ValueType board::getCell(int i, int j) const
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
   if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
      return value[i-1][j-1];
   else
      throw rangeError("bad value in getCell");
}

void board::setCell(int i, int j, int val)
// Sets a value in cell coordinates
// Throws an expection if bad values are passed
{
   if (i >= 0 && i < BoardSize && j >= 0 && j < BoardSize)
   {
	   value[i][j] = val;

	   // Update Conflicts
	   rowConflicts[i][val-1] = true;
	   colConflicts[j][val-1] = true;
	   sqrConflicts[squareNumber(i,j)-1][val-1] = true;
   }
   else
      throw rangeError("bad value in setCell");
}

bool board::isBlank(int i, int j) const
// Returns true if cell i,j is blank, and false otherwise.
{
   if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
      throw rangeError("bad value in isBlank");
   else
   {
	   i--; j--;
	   if (value[i][j] == 0)
		   return true;
	   else
		   return false;
   }
}

void board::print() const
// Prints the current board.
{
   printf("\n\nThere are %d recursions\n\n", recursionCounter);

   for (int i = 1; i <= BoardSize; i++)
   {
      if ((i-1) % SquareSize == 0)
      {
          cout << " -";
		  for (int j = 1; j <= BoardSize; j++)
	 	     cout << "---";
          cout << "-";
		  cout << endl;
      }
      for (int j = 1; j <= BoardSize; j++)
      {
		  if ((j-1) % SquareSize == 0)
	 	     cout << "|";
	 	  if (!isBlank(i,j))
	 	     cout << " " << getCell(i,j) << " ";
	 	  else
	 	     cout << "   ";
      }
      cout << "|";
      cout << endl;
   }

   cout << " -";
   for (int j = 1; j <= BoardSize; j++)
      cout << "---";
   cout << "-";
   cout << endl;
}

void board::printConflicts(int type) const
// Prints out all conflicts
{
	vector< vector<bool> > vb;
	string strType;

	switch (type) 
	{
		case 0:		// Row Conflicts
		{
			vb = rowConflicts;
			strType = "Row";
			break;
		}
		case 1:		// Column Conflicts
		{
			vb = colConflicts;
			strType = "Column";
			break;
		}
		case 2:		// Square Conflicts
		{
			vb = sqrConflicts;
			strType = "Square";
			break;
		}
	}
	for (int i = 0; i < BoardSize; i++)
	{
		cout << strType << " " << i+1 << " Conflicts:\n\t";
		for (int j = 0; j < BoardSize; j++)
		{
			if (vb[i][j])
				cout << j+1 << ", ";

			if (j == BoardSize-1)
				cout << endl;
		}
	}

	cout << endl << endl;
}

bool board::checkConflict(int val, int row, int col)
// Check if there are any row, col, or sqr conflicts
// for a given value (val) and position (row + col)
// returns true if a conflict exists
{
	if (rowConflicts[row][val-1])
		return true;
	if (colConflicts[col][val-1])
		return true;
	if (sqrConflicts[squareNumber(row,col)-1][val-1])
		return true;

	return false;
}

void board::updateCell(int val, int row, int col)
// Place value in cell and update conflicts
{
	// Check if there are existing conflicts for this value and location?
	if (checkConflict(val, row, col))
		return;

	int oldVal = value[row][col];
	if (oldVal != val)
	{
		// Remove Conflicts for previous cell value
		if (oldVal != 0)
		{
			rowConflicts[row][oldVal-1] = false;
			colConflicts[col][oldVal-1] = false;
			sqrConflicts[squareNumber(row,col)-1][oldVal-1] = false;
		}

		// Add Conflicts for new cell value
		rowConflicts[row][val-1] = true;
		colConflicts[col][val-1] = true;
		sqrConflicts[squareNumber(row,col)-1][val-1] = true;
	}
}

void board::clearCell(int row, int col)
// Clear value from cell
{
	int oldVal = value[row][col];
	if (oldVal != 0)
	{
		rowConflicts[row][oldVal-1] = false;
		colConflicts[col][oldVal-1] = false;
		sqrConflicts[squareNumber(row,col)-1][oldVal-1] = false;
	}

	value[row][col] = 0;
}

bool board::isSolved() const
{
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			// If Cell is Empty
			if (value[i][j] == 0)
			{
				//cout << "The board has not been solved!" << endl << endl;
				return false;
			}

			// If there is a conflict
			if (!rowConflicts[i][j])
			{
				//cout << "The board has not been solved!" << endl << endl;
				return false;
			}
			// If there is a conflict
			if (!colConflicts[i][j])
			{
				//cout << "The board has not been solved!" << endl << endl;
				return false;
			}
			/*
			// If there is a conflict
			if (!sqrConflicts[i][j])
			{
				//cout << "The board has not been solved!" << endl << endl;
				return false;
			}
			*/
		}
	}

	cout << "The board has been completely solved!" << endl;
	//print();

	return true;
}

void board::solve()
{
	recursionCounter++;
	int v, i = -1, j = -1;

	if (isSolved())
		print();
	else
	{
		// Find first blank cell (i,j) and loop over all values v
		findFirstBlankCell(i, j);

		// Debugging Break Point
		//if (i == 0 && j == 4)
		//	cout << "reached break point";

		// Print blank cell coords
		//printf("(%d, %d)", i, j);
		//cout << endl;

		for (v=1; v < 10; v++)
		{
			if (i >= 0 && j >= 0 && (!checkConflict(v, i, j)))
			{
				//value[i][j] = v;
				setCell(i, j, v);
				solve();

				// Clear cell while "un-recursing"
				clearCell(i, j);
			}
		}

		// Clear cell while "un-recursing"
		//clearCell(i, j);
	}
}

void board::findFirstBlankCell(int& row, int& col)
{
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			if (value[i][j] == 0)
			{
				//row = i;
				//col = j;
				//return;

			}
		}
	}
}

void board::printNumberOfRecursions()
{
	cout << endl << "There are " << recursionCounter << " recursions" << endl;
}
