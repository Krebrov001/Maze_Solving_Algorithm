#include "stdafx.h"

#include "helper_functions_header.h"

// using declarations
using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::ios;
using std::ifstream;
using std::string;


HANDLE* initializeConsole()
{
	// set the font of the console
	// http://stackoverflow.com/a/35383318/5500589 
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 14;                  // Width of each character in the font
	cfi.dwFontSize.Y = 28;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Lucida Console"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	// set the size of the console window
	// http://stackoverflow.com/a/21238849 
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 900, 800, TRUE); // 900 width, 800 height

	system("title Maze Walker");  // set the tile of the console window

	// The handle to the console allows us to modify the properties of the console during runtime.
	// The HANDLE object is dynamically allocated on the heap because a pointer to it is supposed
	// to be returned from initializeConsole() so that the main() function can use it.
	HANDLE* hConsole = new HANDLE;
	*hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// explicitly set the console text to white
	SetConsoleTextAttribute(*hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

	// Returns the address of the dynamic HANDLE object.
	return hConsole;
}


void drawLoadingScreen()
{
	// full + empty == loadingMax
	int loadingMax = 20;       // The max number of loading positions.
	int full = 0;              // Initially none of the loading positions are full.   
	int empty = loadingMax;    // Initially all of the loading positions are empty.

	do {
		system("CLS");  // Clear the screen.
		cout << endl << endl;
		cout << setw(27) << setfill(' ') << "Loading ..." << endl;
		cout << endl;
		cout << setw(10) << setfill(' ') << "";
		cout << "|";
		cout << setw(full) << setfill('#') << "";
		cout << setw(empty) << setfill('_') << "";
		cout << "|";
		cout << endl;
		Sleep(rand() % 1000 + 1);
		full++;
		empty--;
	} while (full <= loadingMax);

	// At the end of the loop, all of the loading positions are full,
	// and none of the loading positions are empty.
	// The loading bar has covered all of the loading positions to the end.

	return;
}


bool stringIsInteger(string str)
{
	if (str.empty())
		return false;

	string::iterator it;

	// This loop's only purpose is to see whether the iterator
	// can get to the end without being stopped by any non-digit char.
	// If the iterator got to the end of the string, it means that all
	// the chars in the string are numeric ones.
	//
	// The condition that the iterator hasn't gotten off the end of the string
	// is checked first. If the iterator has reached the end, then it cannot be
	// dereferenced because there is no real char at the end iterator location.
	// The first condition becomes false, short-circuits, and prevents dereferencing.
	for (it = str.begin(); it != str.end() && isdigit(*it); ++it)
		;

	if (it == str.end())
		return true;
	else
		return false;
}


uint_fast8_t mapFileFormat_isGood(string fileName)
{
	// Try to open the file name that the user specified.
	ifstream reader(fileName);

	/* Test 1: Does the file even exist? */

	// If the reader was not able to open the file,
	// the return status is 0, meaning that the file does not exist.
	if (!reader.is_open()) {
		return 0;
	}

	string line;    // Holds a string in the file terminated by '\n'.
	string word;    // Holds a string in the file terminated by whitespace.
	int number;     // If word represents an integer, then that integer is stored in number.
	int numElements = 0;  // The total number of elements in the grid of the maze map.
	int numRows = 0;      // The number of rows in the grid of the maze map.
	int numCols = 0;      // The number of columns in the grid of the maze map.

	/* Test 2: Are all elements in the file valid numerical integers in the range [0...3]? */

	// This code scans the entire file until EOF is reahced,
	// trying to find an invalid element.
	// If it does find an invalid element, the file is closed and the checking stops,
	// the return status is 1, meaning that the format is wrong.
	while (!reader.eof()) {
		// Read in a word:
		// Read in all chars into word until you hit a whitespace char.
		reader >> word;

		if (stringIsInteger(word)) {
			number = stoi(word);
			if (!(number >= 0 && number <= 3)) {
				reader.close();
				return 1;
			}
		}
		else {
			reader.close();
			return 1;
		}
		numElements++;
	}

	/* Test 3: Is the maze map file a 25 * 25 grid? */

	// Resets all the condition flags, including the eofbit flag.
	reader.clear();
	// After clearing the eofbit flag, we can use seekg()
	// to move the reader to the beginning of the file.
	reader.seekg(0, ios::beg);
	// This loop reads the file line-by-line or rather row-by-row.
	// Each time a line is read, the number of rows is incremented.
	while (!reader.eof()) {
		getline(reader, line);
		numRows++;
	}

	reader.close();

	// The map file is supposed to be a grid of numbers as elements.
	// Find out the number of columns in the grid by
	// dividing the number of elements in the grid by the number of rows.
	numCols = numElements / numRows;

	// The grid of the maze map should be 25 * 25.
	// If the number of columns and the number of rows do not fit the requirements,
	// then the return status is 1, meaning that the format is wrong.
	//
	// If the number of columns and the number of rows do fit the requirements,
	// the the return status is 2, meaning that the format is good, and the
	// maze map file passed all the tests.
	if (!(numCols == COLUMNS && numRows == ROWS))
		return 1;
	else
		return 2;
}


Color_t getRandomColor()
{
	// Each of these variables will hold one of three colors:
	// red, green, or blue.
	Color_t color1;
	Color_t color2;

	// Pick a random color out of red, green, or blue for color1.
	switch (rand() % 3 + 1) {
	case 1:
		color1 = FOREGROUND_BLUE;
		break;
	case 2:
		color1 = FOREGROUND_RED;
		break;
	case 3:
	default:
		color1 = FOREGROUND_GREEN;
		break;
	}

	// Pick a random color out of red, green, or blue for color2.
	switch (rand() % 3 + 1) {
	case 1:
		color2 = FOREGROUND_BLUE;
		break;
	case 2:
		color2 = FOREGROUND_RED;
		break;
	case 3:
	default:
		color2 = FOREGROUND_GREEN;
		break;
	}

	// The "color values" are bit patterns.
	// ORing them together has the effect of combining the colors.
	//
	// Each color has a 1 in 3 chance of being red, green, or blue.
	// If both colors match, we get one of the three primary colors.
	// If both colors are different, we get one of the following mixed colors:
	// red  | green == yellow
	// red  | blue  == purple
	// blue | green == cyan
	//
	// The colors are also ORed with FOREGROUND_INTENSITY, which makes the color bright.
	return color1 | color2 | FOREGROUND_INTENSITY;
}
