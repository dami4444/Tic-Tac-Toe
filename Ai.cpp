#include "Ai.h"

Ai::Ai(Board* b, int turn)
{
	myTurn = turn;
	board = b;
	winningMovePossible = false;
	winningIn2MovesPossible = false;

	CreateMoveArr();
}

Cell* Ai::RandMove()
{
	//if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	sf::sleep(sf::milliseconds(150));	//Ai move delay

	int i = rand() % board->numRows;
	int j = rand() % board->numCols;
	while (board->cellArr[i][j]->GetState() != 0)
	{
		i = rand() % board->numRows;
		j = rand() % board->numCols;
	}
	if (board->cellArr[i][j]->GetState()==0)
	{
		//board->cellArr[i][j]->onClick(board->gameTurn);
		return board->cellArr[i][j];
	}

	//board->state = board->CheckState();
}

void Ai::Start()
{
	sf::sleep(sf::milliseconds(60));	//Ai move delay

	
	FillMoveArr(myTurn);
	if (winningMovePossible == true)
	{
		BestMove();
	}
	else
	{
		BlockAndMove();
	}
}

void Ai::FillMoveArr(int myTurn)
{
	/*EraseMoveArr();
	CreateMoveArr();*/

	int cellState = 0;
	//int checkingDirection = 0;	//The checkingDirection variable determines the direction of the checked row. Values from 0 to 7 clockwise shifting directions. 0 is up, 1 is up-right, 7 up-left etc.

	for (int i = 0; i < board->numRows; i++) {
		for (int j = 0; j < board->numCols; j++) {
			cellState = board->cellArr[i][j]->GetState();	//Checking the state of every cell
			if (cellState == myTurn * -1)					//If the cell contains opponent's symbol
			{
				for (int checkingDirection = 0; checkingDirection <= 7; checkingDirection++)	//Set all elements of moveDiretions to -1 (winningStreak impossible if starting from this cell)
				{
					*(moveArr[i][j][checkingDirection]) = -1;
				}
			}
			else
			{
				for (int checkingDirection = 0; checkingDirection <= 7; checkingDirection++)	//Checking how much cells from a row of winningStreak (streakCounter==(winningStreak-1)) cells in every direction contain this Ai's symbol.
				{
					int* y = new int(i);
					int* x = new int(j);
					int streakCounter = 0;		//Counts how many symbols (O or X) are there in a row. Max value is winningStreak-1, if it's found this Ai will win in this turn.
					int streakLength = 0;		//Counts how mamy cells were checked in a row, used to stop checking if the row reaches winningStreak length	
					bool impossibleStreak = false;

					while ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
					{
						streakLength++;
						if (board->cellArr[*y][*x]->GetState() == myTurn)
						{
							streakCounter++;
						}
						else if (board->cellArr[*y][*x]->GetState() == myTurn * -1)
						{
							*(moveArr[i][j][checkingDirection]) = -1;
							impossibleStreak = true;
							break;
						}
						if (streakLength == this->board->winningStreak) break;
						board->DirectionCheckNextIter(checkingDirection, x, y);	//while incrementation (for the next iteration)
					}
					if ( !( (*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows) ) ) // check if the end of board wasn't reached before getting to a streakLength of winningStreak
					{
						*(moveArr[i][j][checkingDirection]) = -1;
						impossibleStreak = true;
					}
					else if ( (streakLength == this->board->winningStreak) && (impossibleStreak == false) )
					{
						*(moveArr[i][j][checkingDirection]) = streakCounter; //save the found streakCounter in moveArr
						if ((streakCounter == board->winningStreak - 1) && myTurn == this->myTurn)
						{
							this->winningMovePossible = true;
						}
						//Check if win in 2 moves possible (with a not tight blocked streak)
						if ((streakCounter == board->winningStreak - 2) && myTurn == this->myTurn)
						{
							
							if (TightBlockNeeded(longestStreak({i,j,checkingDirection})))
							{
								this->winningIn2MovesPossible = true;
							}

						}
					}
						
					delete x, y;
				}
			}
		}
	}
}

void Ai::Move(Cell* selectedCell)
{
	selectedCell->onClick(board->gameTurn);
	board->state = board->CheckState();	//It's neccesary to update the board state because the board.onClick() method is not called when Ai plays
}

void Ai::BlockAndMove()
{
	//sf::err() << "BlockAndMove\n";
	int opponentTurn = myTurn * -1;
	FillMoveArr(opponentTurn);

	longestStreaks opponentLngStreaks;
	bool blockNotNeeded = false;

	for (int checkedWinningStreak = board->winningStreak - 1; checkedWinningStreak >= board->winningStreak - 3; checkedWinningStreak--) {
		for (int i = 0; i < board->numRows; i++) {
			for (int j = 0; j < board->numCols; j++) {
				for (int k = 0; k <= 7; k++) {
					if (*(moveArr[i][j][k]) == checkedWinningStreak)
					{
						opponentLngStreaks.push_back({ i, j, k });
					}
				}
			}
		}
		if (!opponentLngStreaks.empty())
		{
			if (checkedWinningStreak == board->winningStreak - 1)
			{
				//At this point we have all the longest streaks of length wS-1
				//Block them if there are any
				bestMoves bstMoves = BestMovesFromStreaks(opponentLngStreaks);
				if (bstMoves.size() == 1)
				{
					//sf::err() << "Block for wS-1\n";
					Move(bstMoves[0]);
					return;
				}
				else if (bstMoves.size() == 0)
				{
					//sf::err() << "Error! There should be a block for wS-1";
					return;
				}
				else if (bstMoves.size() > 1)
				{
					//sf::err() << "Block for wS-1\n";
					int randomIndex = rand() % bstMoves.size();		//Randomly select which of the best moves will be played
					Cell* randomBestMove = bstMoves[randomIndex];
					Move(randomBestMove);
					return;

					//TODO:: Add secondary condition, check if one of the best block is also a best move
					//(or any other worse move, best possble when also blocking with one of the chosen blocks)
				}

			}
			else if (checkedWinningStreak == board->winningStreak - 2 /*&& board->winningStreak>3*/)
			{
				if (board->winningStreak == 3 && OppoiteCornersCheck(opponentLngStreaks) /*opponentLngStreaks.size() == 8*/)
				{
					OppositeCornersBlock(opponentLngStreaks);
					return;
				}
				if(board->winningStreak > 3)
				{
					if (winningIn2MovesPossible)
					{
						BestMove();
						return;
					}
					//DONE//TODO::FIX TIGHT BLOCK, they are not always right next to a streak of symbols
					//At this point we have all the longest streaks of both length wS-2
					//Check if tight block is needed here
					//If not then we also find all streaks of lenth wS-3 and then execute the next if
					bestMoves bstMoves = BestMovesFromStreaks(opponentLngStreaks, true);
					if (bstMoves.size() == 1)
					{
						//sf::err() << "Bloc kfor wS-2\n";
						Move(bstMoves[0]);
						return;
					}
					else if (bstMoves.size() == 0)
					{
						//sf::err() << "TEST warrning! There was no tight block for wS-2";
						//return;
					}
					else if (bstMoves.size() > 1)
					{
						//sf::err() << "Bloc kfor wS-2\n";
						int randomIndex = rand() % bstMoves.size();		//Randomly select which of the best moves will be played
						Cell* randomBestMove = bstMoves[randomIndex];
						Move(randomBestMove);
						return;

						//TODO:: Add secondary condition, check if one of the best block is also a best move
						//(or any other worse move, best possble when also blocking with one of the chosen blocks)
					}
				}
			}
			else if (checkedWinningStreak == board->winningStreak - 3 && board->winningStreak > 4)
			{
				//At this point we have all the longest streaks of both lengths wS-3 and wS-2(if they exist)
				//Check for needed double(or more) block moves
				//Check parallel streaks blocks
				if (ParallelStreaksBlock(opponentLngStreaks)) return;
				//Check perpendicular, diagonal or opposite direction streaks double blocks
				bestMoves bstMoves = BestMovesFromStreaks(opponentLngStreaks,false,8);
				//TODO:: FIX DETECTION, DETECT OBLY WHEN two streaks contain no empty ells inbetween cells
				//DONE//TODO:: FIX WEIGTH DETECTION VALUE
				//DONE//TOFO:: CALCUATE IT based on wS and board size or count only from streaks starting from my symbol
				if (bstMoves.size() == 1)
				{
					//sf::err() << "Bloc kfor wS-3";
					Move(bstMoves[0]);
					return;
				}
				else if (bstMoves.size() == 0)
				{
					//sf::err() << "TEST Warrning! There was no double block for wS-2 or wS-3";
					//return;
				}
				else if (bstMoves.size() > 1)
				{
					//sf::err() << "Bloc kfor wS-3";
					int randomIndex = rand() % bstMoves.size();		//Randomly select which of the best moves will be played
					Cell* randomBestMove = bstMoves[randomIndex];
					Move(randomBestMove);
					return;

					//TODO:: Add secondary condition, check if one of the best block is also a best move
					//(or any other worse move, best possble when also blocking with one of the chosen blocks)
				}

				//If there aren't any double blocks to make, there arent any more block to check
				//The Ai doesn't immediately block wS-2 or less streaks that are tight blocked on one side
				//instead they are used as a secondary condition to choose from multiple best moves
				//execute best move search next
				blockNotNeeded = true;
			}
			
		}
	}

	//Search for best moves here
	BestMove();
	return;

}

bool Ai::ParallelStreaksBlock(longestStreaks lngStreaks)
{
	

	///Add each streak that has at lesast one parallel symbol to the parallelStreaks vector
	longestStreaks parallelStreaks;

	for (int streakNr = 0; streakNr < lngStreaks.size(); streakNr++)
	{
		int i = lngStreaks[streakNr][0];
		int j = lngStreaks[streakNr][1];
		int k = lngStreaks[streakNr][2];
		
		if ((k == 1 or k == 3 or k == 5 or k == 7) and board->cellArr[i][j]->GetState() == myTurn * -1)
		{
			int* y = new int(i);
			int* x = new int(j);

			int streakLength = 0;		//Counts how mamy cells were checked in a row, used to stop checking if the row reaches winningStreak length	
			while ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
			{
				streakLength++;
				if (board->cellArr[*y][*x]->GetState() == myTurn * -1)
				{
					for (int distance = 1; distance <= 2; distance++)
					{
						int* yParallel = new int(*y);
						int* xParallel = new int(*x);
						//ADD CHECKING IF THEY ARE ON BOARD
						if (distance == 1)
						{
							board->DirectionCheckNextIter((k + 2) % 8, xParallel, yParallel);
							if ((*xParallel >= 0) && (*xParallel < board->numCols) && (*yParallel >= 0) && (*yParallel < board->numRows))
							{
								if (board->cellArr[*yParallel][*xParallel]->GetState() == myTurn * -1)
								{
									parallelStreaks.push_back(lngStreaks[streakNr]);
								}
							}
							*yParallel = *y;
							*xParallel = *x;

							board->DirectionCheckNextIter((k - 2) % 8, xParallel, yParallel);
							if ((*xParallel >= 0) && (*xParallel < board->numCols) && (*yParallel >= 0) && (*yParallel < board->numRows))
							{
								if (board->cellArr[*yParallel][*xParallel]->GetState() == myTurn * -1)
								{
									parallelStreaks.push_back(lngStreaks[streakNr]);
								}
							}
						}
						else if (distance == 2)
						{
							board->DirectionCheckNextIter((k + 2) % 8, xParallel, yParallel);
							board->DirectionCheckNextIter((k + 2) % 8, xParallel, yParallel);
							if ((*xParallel >= 0) && (*xParallel < board->numCols) && (*yParallel >= 0) && (*yParallel < board->numRows))
							{
								if (board->cellArr[*yParallel][*xParallel]->GetState() == myTurn * -1)
								{
									parallelStreaks.push_back(lngStreaks[streakNr]);
								}
							}
							*yParallel = *y;
							*xParallel = *x;

							board->DirectionCheckNextIter((k - 2) % 8, xParallel, yParallel);
							board->DirectionCheckNextIter((k - 2) % 8, xParallel, yParallel);
							if ((*xParallel >= 0) && (*xParallel < board->numCols) && (*yParallel >= 0) && (*yParallel < board->numRows))
							{
								if (board->cellArr[*yParallel][*xParallel]->GetState() == myTurn * -1)
								{
									parallelStreaks.push_back(lngStreaks[streakNr]);
								}
							}
						}

						delete xParallel, yParallel;
					}
				}
				if (streakLength == this->board->winningStreak) break;
				board->DirectionCheckNextIter(k, x, y);	//while incrementation (for the next iteration)
			}
			//*(moveArr[i][j][checkingDirection]) = streakCounter;	//save the found streakCounter in moveArr
			delete x, y;
		}
	}

	if (parallelStreaks.size() == 0) return false;
	//else if(parallelStreaks.size() > 0)

	possibleMoves posMoves = AllPossibleMovesFromStreaks(lngStreaks, false, 2);

	if (posMoves.size() == 1)
	{
		Move(posMoves[0]);
		return true;
	}
	else if (posMoves.size() == 0)
	{
		return false;
	}
	else if (posMoves.size() > 1)
	{
		std::vector<int> bstMoveSurroudingCellsFilled;
		int surroudingCellsFilledMax = 0;
		for (int i = 0; i < posMoves.size(); i++)
		{
			
			int surroudingCellsFilled = 0;
			for (int k = 0; k <= 7; k++)
			{
				int* x = new int(posMoves[i]->GetCol());
				int* y = new int(posMoves[i]->GetRow());

				board->DirectionCheckNextIter(k,x,y);
				if ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
				{
					if (board->cellArr[*y][*x]->GetState() == myTurn * -1)
					{
						surroudingCellsFilled++;
					}
				}
				
				delete x, y;
			}
			if (surroudingCellsFilled > surroudingCellsFilledMax) surroudingCellsFilledMax = surroudingCellsFilled;
			bstMoveSurroudingCellsFilled.push_back(surroudingCellsFilled);
		}


		if (surroudingCellsFilledMax < 3) return false;

		bestMoves bstMovesWithMaxSurroundingCells;
		for (int i = 0; i < posMoves.size(); i++)
		{
			if (bstMoveSurroudingCellsFilled[i] == surroudingCellsFilledMax) bstMovesWithMaxSurroundingCells.push_back(posMoves[i]);
		}

		if (surroudingCellsFilledMax == 3)
		{
			bestMoves bestMovesEmptySurroundingCellsInRowWithFilledCell;

			for (int i = 0; i < bstMovesWithMaxSurroundingCells.size(); i++)
			{
				for (int k = 0; k <= 7; k++)
				{
					int* x = new int(bstMovesWithMaxSurroundingCells[i]->GetCol());
					int* y = new int(bstMovesWithMaxSurroundingCells[i]->GetRow());

					board->DirectionCheckNextIter(k, x, y);
					if ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
					{
						if (board->cellArr[*y][*x]->GetState() == myTurn * -1)
						{
							//check the surrounding cell that is opposite to a filled surrounding cell
							int reverseDirection = (k + 4) % 8;
							board->DirectionCheckNextIter(reverseDirection, x, y);
							board->DirectionCheckNextIter(reverseDirection, x, y);
							if ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
							{
								if (board->cellArr[*y][*x]->GetState() == 0)
								{
									//if it's empty add it as a possible move
									bestMovesEmptySurroundingCellsInRowWithFilledCell.push_back(board->cellArr[*y][*x]);
								}
							}

						}
					}
					delete x, y;
				}
			}

			if (bestMovesEmptySurroundingCellsInRowWithFilledCell.size() > 0)
			{
				int randomIndex = rand() % bestMovesEmptySurroundingCellsInRowWithFilledCell.size();		//Randomly select which of the best moves surrounded by most cells filled by the opponent will be played
				Cell* randomBestMove = bestMovesEmptySurroundingCellsInRowWithFilledCell[randomIndex];
				Move(randomBestMove);
				return true;
			}
			else return false;
			
		}
		else /*if (surroudingCellsFilledMax > 3)*/
		{
			int randomIndex = rand() % bstMovesWithMaxSurroundingCells.size();		//Randomly select which of the best moves surrounded by most cells filled by the opponent will be played
			Cell* randomBestMove = bstMovesWithMaxSurroundingCells[randomIndex];
			Move(randomBestMove);
			return true;
		}
		

	}
}

bool Ai::TightBlockNeeded(longestStreak lngStreak)
{
	int i = lngStreak[0];
	int j = lngStreak[1];
	int k = lngStreak[2];

	int* y = new int(i);
	int* x = new int(j);

	////Check if the streak starting cell is opponent's symbol
	if (board->cellArr[*y][*x]->GetState() == myTurn * -1)
	{

		//Check the cell behind streak start
		int reverseDirection = ((k + 4) % 8);
		board->DirectionCheckNextIter(reverseDirection, x, y);
		if ( ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows)) )//if it's on board
		{
			if (board->cellArr[*y][*x]->GetState() == myTurn)//if it is this ai's symbol
			{//IF not on board(no such cell) OR cell symbol is this Ai's symbol
				//no thight block needed for this streak
				delete x, y;
				return false;//no tight block neccesary
			}
		}
		
	}

	//Check the last cell of the streak
	*y = i;
	*x = j;
	for (int z = 1; z < this->board->winningStreak; z++)//skip to the last cell of the streak
	{
		board->DirectionCheckNextIter(k, x, y);
	}
	////Check if the last streak cell is opponent's symbol
	if (board->cellArr[*y][*x]->GetState() == myTurn * -1)
	{

		//Check the cell after streak end
		board->DirectionCheckNextIter(k, x, y);
		if (((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows)))//if it's on board
		{
			if (board->cellArr[*y][*x]->GetState() == myTurn)//if it is this ai's symbol
			{//IF not on board(no such cell) OR cell symbol is this Ai's symbol
				//no thight block needed for this streak
				delete x, y;
				return false;//no tight block neccesary
			}
		}

	}

	//if any of the previous conditions werent met tight block is neccesary
	return true;
}

bestMoves Ai::BestMovesFromStreaks(longestStreaks lngStreaks, bool tightBlockOnly, int minWeigth)
{
	//Finding the empty cells form  all of the longestStreaks and saving them in a cellRow vector
	cellRow allPossibleMoves = AllPossibleMovesFromStreaks(lngStreaks, tightBlockOnly, minWeigth);

	//Removing duplicates and counting how many times each cell(move) appears
	possibleMoves posMoves;
	int numOfPosMove = 0;
	possibleMoveWeigth posMoveWeigth;

	bestMoves bstMoves;
	int maxWeigth = 0;

	for (int i = 0; i < allPossibleMoves.size(); i++)
	{
		bool posMovesContainsThisMove = false;
		for (int j = 0; j < posMoves.size(); j++)
		{
			if (allPossibleMoves[i] == posMoves[j]) posMovesContainsThisMove = true;
		}
		if (posMovesContainsThisMove == false)
		{
			int moveWeigth = std::count(allPossibleMoves.begin(), allPossibleMoves.end(), allPossibleMoves[i]);
			posMoves.push_back(allPossibleMoves[i]);
			posMoveWeigth.push_back(moveWeigth);

			if (moveWeigth > maxWeigth) maxWeigth = moveWeigth;
		}
	}

	//Chosing the best moves from the possibleMoves
	for (int i = 0; i < posMoves.size(); i++)
	{
		if (posMoveWeigth[i] == maxWeigth && posMoveWeigth[i] >= minWeigth) bstMoves.push_back(posMoves[i]);
	}
	
	//returning the best moves/blocks
	return bstMoves;
	
}

cellRow Ai::AllPossibleMovesFromStreaks(longestStreaks lngStreaks, bool tightBlockOnly, int minWeigth)
{
	//Finding the empty cells form  all of the longestStreaks and saving them in a cellRow vector
	int cellState = 0;
	cellRow allPossibleMoves;	//Using the board's cellRow to store the cells from which one will be selected insted of creating a new vector typedef 

	for (int streakNr = 0; streakNr < lngStreaks.size(); streakNr++)
	{
		if (tightBlockOnly == false)
		{
			int i = lngStreaks[streakNr][0];
			int j = lngStreaks[streakNr][1];
			int k = lngStreaks[streakNr][2];

			int* y = new int(i);
			int* x = new int(j);

			int streakLength = 0;		//Counts how mamy cells were checked in a row, used to stop checking if the row reaches winningStreak length	
			while ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
			{
				streakLength++;
				if (board->cellArr[*y][*x]->GetState() == 0) allPossibleMoves.push_back(board->cellArr[*y][*x]);
				if (streakLength == this->board->winningStreak) break;
				board->DirectionCheckNextIter(k, x, y);	//while incrementation (for the next iteration)
			}
			//*(moveArr[i][j][checkingDirection]) = streakCounter;	//save the found streakCounter in moveArr
			delete x, y;
		}
		else if (tightBlockOnly == true)
		{
			if (!TightBlockNeeded(lngStreaks[streakNr]))
			{
				continue;
			}

			bool filledCellFound = false;

			int i = lngStreaks[streakNr][0];
			int j = lngStreaks[streakNr][1];
			int k = lngStreaks[streakNr][2];

			int* y = new int(i);
			int* x = new int(j);

			//Add ONLY the first empty cell in the streak(possible tight block) to allPossibleMoves
			*y = i;
			*x = j;
			int streakLength = 0;		//Counts how mamy cells were checked in a row, used to stop checking if the row reaches winningStreak length	
			while ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
			{
				streakLength++;
				if (board->cellArr[*y][*x]->GetState() == myTurn * -1) filledCellFound = true;
				if (board->cellArr[*y][*x]->GetState() == 0 && filledCellFound)
				{
					allPossibleMoves.push_back(board->cellArr[*y][*x]);
					break;
				}
				if (streakLength == this->board->winningStreak) break;
				board->DirectionCheckNextIter(k, x, y);	//while incrementation (for the next iteration)
			}
			delete x, y;
		}

	}
	return allPossibleMoves;
}

bestMoves Ai::BestMovesFromAllPossibleMovesAndFormerBestMoves(cellRow allPossibleMoves, bestMoves formerBestMoves)
{
	//Removing duplicates and counting how many times each cell(move) appears
	possibleMoves posMoves = formerBestMoves;
	bestMoves bestMovesandBlocks;

	possibleMoveWeigth posMoveWeigth;
	int maxWeigth = 0;

	for (int i = 0; i < posMoves.size(); i++)
	{//Check how many block are on the same cell as each of the possible(former best) moves
		int moveWeigth = std::count(allPossibleMoves.begin(), allPossibleMoves.end(), posMoves[i]);
		//posMoves.push_back(allPossibleMoves[i]);
		posMoveWeigth.push_back(moveWeigth);
		if (moveWeigth > maxWeigth) maxWeigth = moveWeigth;
	}

	//Chosing the best moves from the possibleMoves
	for (int i = 0; i < posMoves.size(); i++)
	{
		if (posMoveWeigth[i] == maxWeigth) bestMovesandBlocks.push_back(posMoves[i]);
	}

	return  bestMovesandBlocks;
}

void Ai::BestMove()
{
	FillMoveArr(myTurn);
	longestStreaks myLngStreaks;

	for (int checkedWinningStreak = board->winningStreak - 1; checkedWinningStreak >= 0; checkedWinningStreak--) {
		for (int i = 0; i < board->numRows; i++) {
			for (int j = 0; j < board->numCols; j++) {
				for (int k = 0; k <= 7; k++) {
					if (*(moveArr[i][j][k]) == checkedWinningStreak)
					{
						myLngStreaks.push_back({ i, j, k });
					}
				}
			}
		}
		if (!myLngStreaks.empty())
		{
			bestMoves bstMoves = BestMovesFromStreaks(myLngStreaks);
			if (bstMoves.size() == 1)
			{
				Move(bstMoves[0]);
				return;
			}
			else if (bstMoves.size() == 0)
			{
				//sf::err() << "TEST warrning! There was no best move for wS of any length";
				Move(RandMove());
				return;
			}
			//(choose a move that is both a best move and a block(probably not tight wS-2 or less block))
			//Use the opponentLongestStreaks vector that is already filled with streaks of length wS-3 and wS-2
			else if (bstMoves.size() > 1)//Secondary condition, check if one of the best movess is also a block
			{

				int opponentTurn = myTurn * -1;
				FillMoveArr(opponentTurn);
				longestStreaks opponentLngStreaks;

				for (int checkedWinningStreak = board->winningStreak - 2; checkedWinningStreak >= board->winningStreak - 2; checkedWinningStreak--) {
					for (int i = 0; i < board->numRows; i++) {
						for (int j = 0; j < board->numCols; j++) {
							for (int k = 0; k <= 7; k++) {
								if (*(moveArr[i][j][k]) == checkedWinningStreak)
								{
									opponentLngStreaks.push_back({ i, j, k });
								}
							}
						}
					}
					if (!opponentLngStreaks.empty())
					{
						//bestMoves bstBlocks = BestMovesFromStreaks(myLngStreaks);
						cellRow allPossibleMoves = AllPossibleMovesFromStreaks(opponentLngStreaks);
						bestMoves bestMovesandBlocks = BestMovesFromAllPossibleMovesAndFormerBestMoves(allPossibleMoves, bstMoves);

						//returning the best moves/blocks
						if (bestMovesandBlocks.size() == 1)
						{
							Move(bestMovesandBlocks[0]);
							return;
						}
						else if (bestMovesandBlocks.size() > 1)//Teritary condition, check if one of the move and blocks is also a move of a shorter longest streak
						{
							FillMoveArr(myTurn);
							longestStreaks teritaryMoveLngStreaks;
							int teritaryMoveWinningStreak = checkedWinningStreak - 1;
							for (int i = 0; i < board->numRows; i++) {
								for (int j = 0; j < board->numCols; j++) {
									for (int k = 0; k <= 7; k++) {
										if (*(moveArr[i][j][k]) == teritaryMoveWinningStreak)
										{
											teritaryMoveLngStreaks.push_back({ i, j, k });
										}
									}
								}
							}
							if (!teritaryMoveLngStreaks.empty())
							{
								cellRow teritaryMovePossibleMoves = AllPossibleMovesFromStreaks(teritaryMoveLngStreaks);
								bestMoves bestMovesandBlocksandTeritaryMoves = BestMovesFromAllPossibleMovesAndFormerBestMoves(teritaryMovePossibleMoves, bestMovesandBlocks);

								if (bestMovesandBlocksandTeritaryMoves.size() == 0)
								{
									Move(bestMovesandBlocks[0]);
									return;
								}
								else if (bestMovesandBlocksandTeritaryMoves.size() > 0)
								{
									int randomIndex = rand() % bestMovesandBlocksandTeritaryMoves.size();		//Randomly select which of the best moves will be played
									Cell* randomBestMove = bestMovesandBlocksandTeritaryMoves[randomIndex];
									Move(randomBestMove);
									return;
								}
							}
							
							int randomIndex = rand() % bestMovesandBlocks.size();		//Randomly select which of the best moves will be played
							Cell* randomBestMove = bestMovesandBlocks[randomIndex];
							Move(randomBestMove);
							return;
						}

						
					}
				}

				int randomIndex = rand() % bstMoves.size();		//Randomly select which of the best moves will be played
				Cell* randomBestMove = bstMoves[randomIndex];
				Move(randomBestMove);
				return;
			}
		}
	}
	Move(RandMove());
}

bool Ai::OppoiteCornersCheck(longestStreaks lngStreaks)
{

	bool TopLeftAndBottomRight = ((board->cellArr[0][0]->GetState() == myTurn * -1) && (board->cellArr[board->numRows - 1][board->numCols - 1]->GetState() == myTurn * -1));
	bool TopRightAndBottomLeft = ((board->cellArr[0][board->numCols - 1]->GetState() == myTurn * -1) && (board->cellArr[board->numRows - 1][0]->GetState() == myTurn * -1));
	if (TopLeftAndBottomRight || TopRightAndBottomLeft)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void Ai::OppositeCornersBlock(longestStreaks lngStreaks)
{
	//Finding the empty cells form  all of the longestStreaks and saving them in a cellRow vector
	//Finding the empty cells form  all of the longestStreaks and saving them in a cellRow vector
	int cellState = 0;
	cellRow allPossibleMoves;	//Using the board's cellRow to store the cells from which one will be selected insted of creating a new vector typedef 

	for (int streakNr = 0; streakNr < lngStreaks.size(); streakNr++)
	{
		/*if (!TightBlockNeeded(lngStreaks[streakNr]))
		{
			continue;
		}*/

		bool filledCellFound = false;

		int i = lngStreaks[streakNr][0];
		int j = lngStreaks[streakNr][1];
		int k = lngStreaks[streakNr][2];

		int* y = new int(i);
		int* x = new int(j);

		//Add ONLY the first empty cell in the streak(possible tight block) to allPossibleMoves
		*y = i;
		*x = j;
		int streakLength = 0;		//Counts how mamy cells were checked in a row, used to stop checking if the row reaches winningStreak length	
		while ((*x >= 0) && (*x < board->numCols) && (*y >= 0) && (*y < board->numRows))
		{
			streakLength++;
			if (board->cellArr[*y][*x]->GetState() == myTurn * -1) filledCellFound = true;
			if (board->cellArr[*y][*x]->GetState() == 0 && filledCellFound)
			{
				allPossibleMoves.push_back(board->cellArr[*y][*x]);
				break;
			}
			if (streakLength == this->board->winningStreak) break;
			board->DirectionCheckNextIter(k, x, y);	//while incrementation (for the next iteration)
		}
		delete x, y;

	}

	//Removing duplicates and counting how many times each cell(move) appears
	possibleMoves posMoves;
	int numOfPosMove = 0;
	possibleMoveWeigth posMoveWeigth;

	bestMoves bstMoves;
	int maxWeigth = 0;

	for (int i = 0; i < allPossibleMoves.size(); i++)
	{
		bool posMovesContainsThisMove = false;
		for (int j = 0; j < posMoves.size(); j++)
		{
			if (allPossibleMoves[i] == posMoves[j]) posMovesContainsThisMove = true;
		}
		if (posMovesContainsThisMove == false)
		{
			int moveWeigth = std::count(allPossibleMoves.begin(), allPossibleMoves.end(), allPossibleMoves[i]);
			posMoves.push_back(allPossibleMoves[i]);
			posMoveWeigth.push_back(moveWeigth);

			if (moveWeigth > maxWeigth) maxWeigth = moveWeigth;
		}
	}

	//Chosing the best moves from the possibleMoves
	for (int i = 0; i < posMoves.size(); i++)
	{
		if (posMoveWeigth[i] == maxWeigth) bstMoves.push_back(posMoves[i]);
	}

	//making the double corner block
	if (bstMoves.size() == 1)
	{
		Move(bstMoves[0]);
		return;
	}
	else if (bstMoves.size() > 1)
	{
		int randomIndex = rand() % bstMoves.size();		//Randomly select which of the best moves will be played
		Cell* randomBestMove = bstMoves[randomIndex];
		Move(randomBestMove);
		return;
	}
}

Ai::~Ai()
{
	for (int i = 0; i < board->numRows; i++) {
		for (int j = 0; j < board->numCols; j++) {
			for (int k = 0; k <= 7; k++) {
				delete moveArr[i][j][k];
			}
		}
	}
	moveArr.clear();
}

void Ai::EraseMoveArr()
{
	for (int i = 0; i < board->numRows; i++) {
		for (int j = 0; j < board->numCols; j++) {
			for (int k = 0; k <= 7; k++) {
				delete moveArr[i][j][k];
			}
		}
	}
	moveArr.clear();
}

void Ai::CreateMoveArr()
{

	// Add rows
	for (int i = 0; i < board->numRows; i++) {
		moveArr.push_back(moveRow());
	}

	// Add cells (cells' moveDirecrtions vectors) for each row
	for (int i = 0; i < board->numRows; i++) {
		for (int j = 0; j < board->numCols; j++) {
			moveArr[i].push_back(moveDirections());
		}
	}

	//Add move directions for each cell

	for (int i = 0; i < board->numRows; i++) {
		for (int j = 0; j < board->numCols; j++) {
			for (int k = 0; k <= 7; k++) {
				moveArr[i][j].push_back(new int);
			}
		}
	}

}
