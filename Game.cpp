#include "stdafx.h"
#include "Game.h"


Game::Game()
{
	// The number of steps taken by the AI is initially 0.
	steps = 0;

	// Set the status of the game as true (running)
	gameStatus = true;
	// The maze has not been solved yet.
	mazeSolved = false;

	/* The defaults for the startLocation, finishLocation, and AILocation are (0, 0) */
	startLocation  = { 0, 0 };
	finishLocation = { 0, 0 };
	AILocation     = { 0, 0 };

	neighborLocations = new Coordinates[4];
	// Left neighboring location.
	neighborLocations[0].x = -1;
	neighborLocations[0].y = 0;
	// Up neighboring location.
	neighborLocations[1].x = 0;
	neighborLocations[1].y = -1;
	// Right neighboring location.
	neighborLocations[2].x = +1;
	neighborLocations[2].y = 0;
	// Down neighboring location.
	neighborLocations[3].x = 0;
	neighborLocations[3].y = +1;

	foundLocations = new Coordinates[3];
	// Initially, set each element of foundLocations as an EMPTY_LOCATION.
	// foundLocations is empty at the start, and contains no meaningful data.
	for (size_t i = 0; i != 3; ++i) {
		foundLocations[i].x = EMPTY_LOCATION;
		foundLocations[i].y = EMPTY_LOCATION;
	}
	
	// These are no possible choices at the start.
	numChoices = 0;

	/* These four dynamic 2-D arrays, which hold information about the state
	 * of the maze map, all together represent a single data structure.
	 * This data structure can be visualized as a multi-bool grid where each element
	 * stores four possible true/false pieces of information about the grid location at that position.
	 *
	 * I want to allocate this unified "multi-bool" data structure as one whole thing in memory.
	 * I am dynamically allocating all the 2-D bool arrays in the heap together in one step.
	 */
	isEmpty    = new bool*[ROWS];
	hasVisited = new bool*[ROWS];
	isCross    = new bool*[ROWS];
	deadEnd    = new bool*[ROWS];
	for (size_t i = 0; i != ROWS; ++i) {
		*(isEmpty    + i) = new bool[COLUMNS];
		*(hasVisited + i) = new bool[COLUMNS];
		*(isCross    + i) = new bool[COLUMNS];
		*(deadEnd    + i) = new bool[COLUMNS];
	}
}


Game::~Game()
{
	// Deallocate the dynamic neighborLocations array of Coordinates structs.
	delete[] neighborLocations;

	// Deallocate the dynamic foundLocations array of Coordinates structs.
	delete[] foundLocations;

	/* Deallocate the dynamic unified "multi-bool" grid data structure.
	 * Deallocate the dynamic 2-D bool arrays all together in one strp.
	 */
	for (size_t i = 0; i != ROWS; ++i) {
		delete[] *(isEmpty + i);
		delete[] *(hasVisited + i);
		delete[] *(isCross + i);
		delete[] *(deadEnd + i);
		*(isEmpty + i)    = nullptr;
		*(hasVisited + i) = nullptr;
		*(isCross + i)    = nullptr;
		*(deadEnd + i)    = nullptr;
	}
	delete[] isEmpty;
	delete[] hasVisited;
	delete[] isCross;
	delete[] deadEnd;
	isEmpty    = nullptr;
	hasVisited = nullptr;
	isCross    = nullptr;
	deadEnd    = nullptr;
}


void Game::readLevel(int** MazeMap) {

	for (int y = 0; y != ROWS; ++y) {
		for (int x = 0; x != COLUMNS; ++x) {

			switch (MazeMap[y][x]) {
			// A wall.
			case 1:
				isEmpty[y][x] = false;
				break;
			// The starting location in the maze.
			case 2:
				isEmpty[y][x] = true;
				startLocation.y = y;
				startLocation.x = x;
				break;
			// The finish location in the maze.
			case 3:
				isEmpty[y][x] = true;
				finishLocation.y = y;
				finishLocation.x = x;
				break;
			// A plain empty space.
			case 0:
			default:
				isEmpty[y][x] = true;
				break;
			}

			hasVisited[y][x] = false;
			deadEnd[y][x] = false;
			isCross[y][x] = false;
		}
	}

	// Sets the AI location to the starting location of the maze
	// after finished reading the maze map.
	setAILocation();

	return;
}


void Game::PrintLevel() const {
	system("CLS");  // Clear the screen.

	for (int y = 0; y != ROWS; ++y) {
		for (int x = 0; x != COLUMNS; ++x) {

			// print a wall block
			if (isEmpty[y][x] == false) {
				cout << "\333\333";
			}
			// print the AI in its current location
			else if ((AILocation.y == y) && (AILocation.x == x)) {
				cout << "\2 ";
			}
			// print an empty space
			else if (isEmpty[y][x] == true) {
				cout << "  ";
			}

		}
		cout << endl;
	}
}


void Game::setAILocation() {
	AILocation.x = startLocation.x;
	AILocation.y = startLocation.y;
}


void Game::setAILocation(int y, int x) {
	// Sets the former AI location as visited.
	hasVisited[AILocation.y][AILocation.x] = true;

	// Moves the AI into the next location.
	AILocation.y = y;
	AILocation.x = x;

	// increment the steps counter
	steps++;
}


void Game::setGameStatus(bool gameStatus) {
	// this-> is used to avoid name conflict with the formal parameter.
	this->gameStatus = gameStatus;
}


bool Game::getGameStatus() const {
	return gameStatus;
}


bool Game::getMazeSolved() const {
	return mazeSolved;
}


int Game::getSteps() const {
	return steps;
}


/**
 * @return bool - true if the grid location at (x, y) is empty (not a wall),
 *                false otherwise.
 */
bool Game::getEmptyStatus(int y, int x) const {
	return isEmpty[y][x];
}


/**
* @return bool - true if the grid location at (x, y) had already been visited by the AI
*                false otherwise.
*/
bool Game::getVisitStatus(int y, int x) const {
	return hasVisited[y][x];
}


/**
* @return bool - true if the grid location at (x, y) is marked as a dead end,
*                false otherwise.
*/
bool Game::getEndStatus(int y, int x) const {
	return deadEnd[y][x];
}


/**
* @return bool - true if the grid location at (x, y) is a cross, or intersection,
*                from which the AI can choose multiple different paths to travel on,
*                false otherwise.
*/
bool Game::getCrossStatus(int y, int x) const {
	return isCross[y][x];
}


int Game::findNextOpenLocation() const {
	int index;

	// Stop searching when you find an empty location.
	for (index = 0; index != 3; ++index) {
		if (foundLocations[index].x == EMPTY_LOCATION) {
			break;
		}
	}

	return index;
}
