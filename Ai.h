#pragma once
#include "Board.h"


typedef std::vector<int*> moveDirections;		//Vector containing pointers to integers that determine how good a move in each direction is. Should contain 8 elements corresponding to the 8 directions, each of them may have values ranging from -1 to (winningStreak-1) of the board. 0 means all cells (including the cell that is the starting point) in this direction are empty. Positive values mean that there are as many cells with the symbol the Ai plays as the value. -1 means that a winning streak in this direction is impossible because it either contains a cell with the opponent's symbol or reaches behind the end of the board.
typedef std::vector<moveDirections> moveRow;	//Vector containing moveDirection(Array containing pointers to integers that determine how good a move in each direction is.) element for each cell in one of the board's rows.
typedef std::vector<moveRow> moveArray;			//Vector cointining moveRow elements for each for of the board's rows.

//Used in the move selecting methods
typedef std::vector<int> longestStreak;			//Vector containing the indexes that identify a streak from moveArray
typedef std::vector<longestStreak> longestStreaks;	//Vector containing longestStreak vectors

typedef std::vector<Cell*> possibleMoves;		//Vector of all the considered possible moves/blocks without repetitions 
typedef std::vector<int> possibleMoveWeigth;	//Vector containing integrs counting how many tmes a move/block repeats
typedef std::vector<Cell*> bestMoves;			//Vector of best moves/block (the ones that have the highest weigth)

class Ai
{
	int myTurn;	//Determines if this Ai's turn is when the board's turn paramter is 1 or -1.
	Board* board;
	moveArray moveArr;
	bool winningMovePossible;
	bool winningIn2MovesPossible;
public:
	Ai(Board* b, int turn);
	Cell* RandMove();
	void Start();
	void FillMoveArr(int myTurn);
	void Move(Cell* selectedCell);

	void BlockAndMove();
	bool ParallelStreaksBlock(longestStreaks lngStreaks);
	bool TightBlockNeeded(longestStreak lngStreak);
	bestMoves BestMovesFromStreaks(longestStreaks lngStreaks, bool tightBlockOnly = false, int minWeigth = 1);
	cellRow AllPossibleMovesFromStreaks(longestStreaks lngStreaks, bool tightBlockOnly = false, int minWeigth = 1);
	bestMoves BestMovesFromAllPossibleMovesAndFormerBestMoves(cellRow allPossibleMoves, bestMoves formerBestMoves);
	void BestMove();

	bool OppoiteCornersCheck(longestStreaks lngStreaks);
	void OppositeCornersBlock( longestStreaks lngStreaks);
	~Ai();
	void EraseMoveArr();
	void CreateMoveArr();
};