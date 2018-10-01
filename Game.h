#ifndef GAME_H
#define GAME_H

/**
 * This struct represents an ordered pair (x, y)
 */
struct Coordinates {
	int x;
	int y;
};

/* These global constants define the number of rows and columns in the grid.
 * They are defined in the file helper_functions_header.h
 * I am extending their scope into this file using extern.
 */
extern const int ROWS;
extern const int COLUMNS;

// Denotes an empty element in foundLocations array.
#define EMPTY_LOCATION -1


class Game
{
public:
	/* The public member functions make up the interface of the Game class. */

	/**
	* The default Game constructor sets the initial state of all the variables in the Game.
	* It takes no parameters.
	*
	* It dynamically creates all the data structures used in the game.
	* These data structures are multidimensional arrays.
	*/
	Game();


	/**
	* The Game class destructor deallocates the dynamic data structures
	* associated with the Game object.
	*/
	~Game();


	/**
	* This function reads the data for the level in from the maze map representation in memory,
	* and sets the initial values of the data structures used in the Game based on the
	* information contained in the maze map.
	*
	* @param int** MazeMap - A dynamic 2-D grid, or array, representing the maze map.
	*/
	void readLevel(int ** MapFile);


	/**
	* This function prints a "frame", the current state of the game
	* for the most recent step.
	*
	* The function reads the data structures of the game and it prints
	* the current location of the AI in the maze map.
	*/
	void PrintLevel() const;


	/**
	* Returns the current status of the game.
	*
	* @return bool - true if the game is still running
	*                false if the game has already finished running
	*/
	bool getGameStatus() const;


	/**
	* Return the mazeSolved status of the game.
	*
	* @return bool - true if the maze was solved,
	*                false otherwise
	*/
	bool getMazeSolved() const;


	/**
	* Returns the value of the steps counter.
	*
	* @return int - The number of steps it took the AI to solve the maze,
	*               including backtracking.
	*/
	int getSteps() const;

private:
	/* The private helper functions. */

	/**
	* This default version of setAILocation with no parameters
	* sets the initial location of the AI to the start location.
	*/
	void setAILocation();


	/**
	* This overloaded version of setAILocation moves the AI into the
	* location specified by the parameters.
	*
	* @param int y - The y-coordinate of the next location.
	* @param int x - The x-coordinate of the next location.
	*
	* This function also sets the former location of the AI as visited
	* and increments the steps counter.
	*/
	void setAILocation(int y, int x);


	/**
	* Sets the status of the game.
	*
	* @param bool gameStatus - The new status of the game.
	*/
	void setGameStatus(bool gameStatus);


	/**
	* @return bool - true if the grid location at (x, y) is empty (not a wall),
	*                false otherwise.
	*/
	bool getEmptyStatus(int y, int x) const;


	/**
	* @return bool - true if the grid location at (x, y) had already been visited by the AI
	*                false otherwise.
	*/
	bool getVisitStatus(int y, int x) const;


	/**
	* @return bool - true if the grid location at (x, y) is marked as a dead end,
	*                false otherwise.
	*/
	bool getEndStatus(int y, int x) const;


	/**
	* @return bool - true if the grid location at (x, y) is a cross, or intersection,
	*                from which the AI can choose multiple different paths to travel on,
	*                false otherwise.
	*/
	bool getCrossStatus(int y, int x) const;


	/**
	* Searches for the next empty location in the foundLocations array.
	*
	* Whenever a possible next position for the AI in the maze is found, that coordinate (x, y)
	* position is written into the first open index in the foundLocations array.
	*
	* @return int - This function returns the index of the first open location in the
	*               foundLocations array into which to store the (x, y) coordinate of the
	*               possible next position.
	*/
	int findNextOpenLocation() const;


	/* The private data members. */

	// The number of steps that the AI took to solve the maze.
	int steps;

	// true while the game is running,
	// becomes false when the game finishes running.
	bool gameStatus;

	// true if the maze was solved by the algorithm at the end of the game.
	// false if the algorithm has not solved the maze.
	bool mazeSolved;

	// The starting position of the AI in the maze grid.
	Coordinates startLocation;

	// The finish position of the AI in the maze grid.
	Coordinates finishLocation;

	// The current position of the AI in the maze grid.
	Coordinates AILocation;

	// A dynamic array of four Coordinates representing the offsets of the neighboring locations
	// from the current locations.
	// neighborLocations[0] - left neighbor
	// neighborLocations[1] - up neighbor
	// neighborLocations[2] - right neighbor
	// neighborLocations[3] - down neighbor
	//
	// Add any of these offsets to the current location to find the real location of
	// the neighbors in the grid.
	Coordinates* neighborLocations;
	
	// This dynamic array stores the actual (x, y) coordinates of the found locations which are
	// considered as being possible next positions for the AI in the next step.
	//
	// When a possible next position for the AI is found, its actual (x, y) coordinate
	// location in the grid is calculated and added to the next empty element in foundLocations.
	//
	// This dynamic array can store up to three Coordinates objects,
	// because the max possible number of empty locations for the AI to move into is three.
	// This occurs when the AI reaches a T-interesction.
	Coordinates* foundLocations;

	// When the AI reaches a (cross, intersection) fork in the road,
	// this variable stores the number of different choices of paths that
	// the AI can travel on, which it has not visited yet.
	int numChoices;

	// Dynamic 2-D array of bools where an element is true if
	// that particular grid location is an empty spot where the AI can travel on.
	bool** isEmpty;

	// Dynamic 2-D array of bools where an element is true if
	// the AI had already been at that particular grid location.
	bool** hasVisited;

	// Dynamic 2-D array of bools where an element is true if
	// that particular grid location is a cross or intersection, a fork in the road.
	bool** isCross;

	// Dynamic 2-D array of bools where an element is true if
	// that particular grid location has been marked as a "dead end".
	bool** deadEnd;
};

#endif /* GAME_H */