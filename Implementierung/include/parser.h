/** @file parser.h
 *  @brief Function prototypes for the implementation of framework program
 *
 *  @author Ngoc Kim Ngan Nguyen (ge23fak)
 */

#ifndef IMPLEMENTIERUNG_PARSER_H
#define IMPLEMENTIERUNG_PARSER_H

/**
 * @brief Print out usage description to the console
 */
void print_usage(void);

/**
 * @brief Print out description of all options and program's usage to the console
 */
void print_help(void);

/**
 * @brief Print out a brief explanation of program's usage and terminate the program with EXIT_FAILURE
 */
void exit_failure(void);

/**
 * @brief Print out an array with the length n and type float (db = 0) or double (db = 1) to the console
 *
 * @param db db = 0 if considering float; db = 1 if considering double
 * @param n Number of values to in input array
 * @param out The given array to be printed out
 */
void print_out(int db, size_t n, void *out);

/**
 * @brief Count the total number of lines in the file given by path
 *
 * @details The method checks if the file given by path is a regular file and returns the total number of lines in it.
 *
 * @param path Path to a file
 */
size_t sizeReadFile(const char *path);

/**
 * @brief Read numbers from a the file given by path and return a pointer to an array storing these numbers
 *
 * @details The method reads each line of the file given by path, converts the read string from each line to a float/double,
 * checks if the converted value is valid, stores all converted values in an array and returns a pointer to this array.
 *
 * @param db db = 0 if considering float; db = 1 if considering double
 * @param count Number of values to be stored in returned array
 * @param path Path to a file
 */
void *readFile(int db, size_t count, const char *path);

/**
 * @brief Read floating point numbers directly from terminal and return a pointer to an array storing these numbers
 *
 * @details The method reads each positional argument passed to the program, converts the read string from each line to a float/double,
 * checks if the converted value is valid, stores all converted values in an array and returns a pointer to this array.
 *
 * @param db db = 0 if considering float; db = 1 if considering double
 * @param argc Number of arguments on the command line
 * @param argv Arguments passed to the program through the command line
 */
void *readTerminal(int db, int argc, char *argv[]);

/**
 * @brief Execute the function specified by version_name and type float/double with arguments n, vals in "loop" iterations
 *
 * @details The method allocates memory space for output array with given length n,
 * executes the function specified by version_name and type float/double with three arguments n, vals, and the new allocated array.
 * The function will be executed loop-times and the method returns the total runtime of these iterations.
 *
 * @param db db = 0 if considering float; db = 1 if considering double
 * @param version_name Name of the funtion to be executed
 * @param n Number of values to be used as input
 * @param vals Pointer to the input array
 * @param loop Number of function iterations to run
 */
double execute(int db, const char *version_name, size_t n, void *vals, int loop);

#endif // IMPLEMENTIERUNG_PARSER_H
