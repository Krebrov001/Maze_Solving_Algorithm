#ifndef HELPER_FUNCTIONS_HEADER_H
#define HELPER_FUNCTIONS_HEADER_H

#include "stdafx.h"


/* These global constants define the number of rows and columns in the grid. */
const int ROWS    = 25;
const int COLUMNS = 25;


// http://stackoverflow.com/a/38086124
/* These macros are defined in windows.h - They are part of the Windows API.
 * They can be represented as 16-bit values.
 * These macros represent the colors of the foreground (text) and the background of the console.
 * They can be bitwise ORed together to combine their effects.
 *	FOREGROUND_BLUE
 *	FOREGROUND_GREEN
 *	FOREGROUND_RED
 *	FOREGROUND_INTENSITY
 *	BACKGROUND_BLUE
 *	BACKGROUND_GREEN
 *	BACKGROUND_RED
 *	BACKGROUND_INTENSITY
 */

// An unsigned 16-bit integer data type can store a "color value" which specifies
// the foreground (text) and background color of the console.
typedef uint16_t Color_t;


/**
* This function sets up the initial properties of the console.
*
* These properties include:
*  - font of the text on the console
*  - size of the console window
*  - title of the console window
*  - foreground (text) and background color of the console
*
* @return HANDLE* - This function creates a new dynamic HANDLE object in the heap,
*                   and attaches it to the console to be able to manipulate its properties.
*      A pointer to the dynamic HANDLE object is returned to the main() function.
*      The main() function uses the HANDLE to manipulate the properties of the console.
*      The main() function is repsonsible for deleting the HANDLE object when it is done using it.
*/
HANDLE* initializeConsole();


/**
* Create the loading text screen animation.
*
* Each iteration of the loop represents a "frame" in the loading text animation.
* In each iteration of the loop:
*  - First the full chars are printed.
*  - Then the empty chars are printed in the remainder of the loadingMax space.
* I am using setw() in conjunction with setfill() to prints the chars without using for-loops.
*
* After each "frame" of the animation is printed in each iteration,
* the program Sleeps for a random duration anywhere from 1 milisecond to 1 second.
* This is to create the effect of a varying loading speed.
*/
void drawLoadingScreen();


/**
* Checks if the passed in string can be converted to an integer.
* This function does not accept signs, hex digits, or real numbers.
*
* @param std::string str - This string should hold an integer value.
*
* @return bool - true if all the characters in the string are digits.
*                false otherwise.
*/
bool stringIsInteger(std::string str);


/**
* This function checks if the format of the maze map file is good.
* A maze map file is good if it is a 25 * 25 grid of integer values [0...3] inclusive,
* each element being separated by whitespace.
*
* A maze map file is bad if:
* 1. it can't be opened
* 2. The elements are not integer values [0...3] inclusive
* 3. The maze map file is not a 25 * 25 grid.
* A maze map file is good if it passes all three tests.
*
* @param std::string filename - The name of the maze map file.
*
* @return uint_fast8_t - This function returns one of three diagnostic values:
*                        0 means that the file could not be opened (probably it doesn't exist)
*                        1 means that the file was opened but it had an incorrect format.
*                        2 means that the maze map file passed all the tests and it is good.
*  These return values are used by the main() function to display different messages to the user.
*  Since the return values can only be 0, 1, or 2, they can be represented as a fast unsigned 8-bit integer.
*  Returning a 32-bit int is just not as efficient, as besides I wanted to use a C standard integer
*  data type for a change.
*/
uint_fast8_t mapFileFormat_isGood(std::string fileName);


/**
* This function returns a random foreground (text) color for the console out of:
*  - bright red
*  - bright blue
*  - bright green
*  - bright yellow
*  - bright purple
*  - bright cyan
*
* @return Color_t - Each of the colors is represented as a 16-bit unsigned integer.
*/
Color_t getRandomColor();

#endif /* HELPER_FUNCTIONS_HEADER_H */