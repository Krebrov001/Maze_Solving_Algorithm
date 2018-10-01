
class Game {
public:
	Game() {
		steps = 0;
		gameStatus = true;      // status of the game
		for (int y = 0; y < (sizeof(foundLocations) / sizeof(*foundLocations)); y++) {
			for (int x = 0; x < (sizeof(foundLocations[0]) / sizeof(*foundLocations[0])); x++) {
				foundLocations[y][x] = 100; // "empty" value
			}
		}
		numChoices = 0;
		mazeSolved = false;
	}
	void readLevel(int MapFile[25][25]) {
		for (int y = 0; y < 25; y++) {
			for (int x = 0; x < 25; x++) {
				switch (MapFile[y][x]) {
				case 1:
					isEmpty[y][x] = false;
					break;
				case 2:
					isEmpty[y][x] = true;
					startLocation[0] = y;
					startLocation[1] = x;
					break;	
				case 3:
					isEmpty[y][x] = true;
					finishLocation[0] = y;
					finishLocation[1] = x;
					break;
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
	}
	void setAILocation() {
		AILocation[0] = startLocation[0];
		AILocation[1] = startLocation[1];
		steps = 0;
	}
	void setGameStatus(bool gameStatus) {
		this->gameStatus = gameStatus;
	}
	bool getGameStatus() const {
		return gameStatus;
	}
	bool getMazeSolved() const {
		return mazeSolved;
	}
	int getSteps() const {
		return steps;
	}
	void PrintLevel() const {
		system("CLS");
		for (int y = 0; y < 25; y++) {
			for (int x = 0; x < 25; x++) {
				if (isEmpty[y][x] == false) {
					cout << "\333\333";
				}
				else if ((AILocation[0] == y) && (AILocation[1] == x)) {
					cout << "\2 ";
				}
				else if (isEmpty[y][x] == true) {
					cout << "  ";
				}
			}
			cout << endl;
		}
	}
	void nextMove() {
		for (int y = 0; y < (sizeof(foundLocations) / sizeof(*foundLocations)); y++) {
			for (int x = 0; x < (sizeof(foundLocations[0]) / sizeof(*foundLocations[0])); x++) {
				foundLocations[y][x] = 100; // "empty" value
			}
		}
		if ((AILocation[0] == startLocation[0]) && (AILocation[1] == startLocation[1])) {
			if (steps == 0) {
				setAILocation((AILocation[0] + 1), AILocation[1]); // move down
			}
			else {
				mazeSolved = false;
				gameStatus = false;
			}
		}
		else if ((AILocation[0] == finishLocation[0]) && (AILocation[1] == finishLocation[1])) {
			mazeSolved = true;
			gameStatus = false;
		}
		else {
			numChoices = 0;
			int tempVal;
			// loop through all possible coordinates for the next location
			for (int y = 0; y < (sizeof(totalLocations) / sizeof(*totalLocations)); y++) {
				// searches for: empty, not visited yet, not marked as dead end
				if (getEmptyStatus((AILocation[0] + totalLocations[y][0]), (AILocation[1] + totalLocations[y][1])) &&
					!getEndStatus((AILocation[0] + totalLocations[y][0]), (AILocation[1] + totalLocations[y][1])) &&
					!getVisitStatus((AILocation[0] + totalLocations[y][0]), (AILocation[1] + totalLocations[y][1]))) {
					tempVal = findNextOpenLocation();              // saves each found next coordinate
					foundLocations[tempVal][0] = (AILocation[0] + totalLocations[y][0]); // y location
					foundLocations[tempVal][1] = (AILocation[1] + totalLocations[y][1]); // x location
					numChoices++;
				}
			}
			if (numChoices == 1) {
				setAILocation(foundLocations[0][0], foundLocations[0][1]);
			}
			else if (numChoices == 2) {
				isCross[AILocation[0]][AILocation[1]] = true;
				srand(time(0));
				rand(); // dispose the first random number, quirk of Visual Studio
				int randomNumber = rand() % 4 + 1;
				if (randomNumber >= 1 && randomNumber <= 2) {
					setAILocation(foundLocations[0][0], foundLocations[0][1]);
				}
				else if (randomNumber >= 3 && randomNumber <= 4) {
					setAILocation(foundLocations[1][0], foundLocations[1][1]);
				}
			}
			else if (numChoices == 3) {
				isCross[AILocation[0]][AILocation[1]] = true;
				srand(time(0));
				rand(); // dispose the first random number, quirk of Visual Studio
				int randomNumber = rand() % 9 + 1;
				if (randomNumber >= 1 && randomNumber <= 3) {
					setAILocation(foundLocations[0][0], foundLocations[0][1]);
				}
				else if (randomNumber >= 4 && randomNumber <= 6) {
					setAILocation(foundLocations[1][0], foundLocations[1][1]);
				}
				else if (randomNumber >= 7 && randomNumber <= 9) {
					setAILocation(foundLocations[2][0], foundLocations[2][1]);
				}
			}
			else if (numChoices == 0) {
				deadEnd[AILocation[0]][AILocation[1]] = true;
				// loop through all possible coordinates for the next location
				for (int y = 0; y < (sizeof(totalLocations) / sizeof(*totalLocations)); y++) {
					// searches for: empty, already visited, not marked as dead end, or cross
					if (getEmptyStatus((AILocation[0] + totalLocations[y][0]), (AILocation[1] + totalLocations[y][1])) &&
						!getEndStatus((AILocation[0] + totalLocations[y][0]), (AILocation[1] + totalLocations[y][1])) &&
						getVisitStatus((AILocation[0] + totalLocations[y][0]), (AILocation[1] + totalLocations[y][1])) ||
						getCrossStatus((AILocation[0] + totalLocations[y][0]), (AILocation[1] + totalLocations[y][1]))) {
						tempVal = findNextOpenLocation();              // saves each found next coordinate
						foundLocations[tempVal][0] = (AILocation[0] + totalLocations[y][0]); // y location
						foundLocations[tempVal][1] = (AILocation[1] + totalLocations[y][1]); // x location
					}
				}
				setAILocation(foundLocations[0][0], foundLocations[0][1]);
			}
			else {
				gameStatus = false;
			}
		}
	}
private:
	void setAILocation(int y, int x) {
		if (hasVisited[AILocation[0]][AILocation[1]] == false) {
			hasVisited[AILocation[0]][AILocation[1]] = true;
		}
		AILocation[0] = y;
		AILocation[1] = x;
		steps++;
	}
	bool getEmptyStatus(int y, int x) const {
		return isEmpty[y][x];
	}
	bool getVisitStatus(int y, int x) const {
		return hasVisited[y][x];
	}
	bool getEndStatus(int y, int x) const {
		return deadEnd[y][x];
	}
	bool getCrossStatus(int y, int x) const {
		return isCross[y][x];
	}
	int findNextOpenLocation() const {
		int y;
		for (y = 0; y < (sizeof(foundLocations) / sizeof(*foundLocations)); y++) {
			if (foundLocations[y][0] == 100) {
				break;
			}
		}
		return y;
	}
	int steps;            // steps taken
	bool gameStatus;      // status of the game
	bool mazeSolved;      // is maze solved
	bool isEmpty[25][25]; // an empty space
	int startLocation[2];    // the starting point
	int finishLocation[2];   // the finish point
	int AILocation[2];       // current position of AI
							 // possible locations for moving
	int totalLocations[4][2] = { { 0, -1 },   // left
	                             { -1, 0 },   // up
	                             { 0, +1 },   // right
	                             { +1, 0 } }; // down
	int foundLocations[3][2]; // actually open locations
	int numChoices;           // number of open locations
	bool hasVisited[25][25]; // already visited
	bool isCross[25][25]; // is intersection
	bool deadEnd[25][25]; // dead end
};
